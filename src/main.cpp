// MIT License
//
// Copyright (c) 2024 devel29a
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "dr_api.h"
#include "drsyms.h"

#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#else
#include <x86intrin.h>
#endif

#ifdef LINUX
#include <sys/time.h>
#include <signal.h>
#endif

#include "profiler.hpp"

namespace
{
    CoMe::Profiler profiler;
#ifdef LINUX
    using Func = int(*)(int, const itimerval*, itimerval*);
    Func enable_prof_timer = nullptr;
#endif
    const std::uint64_t DEFAULT_SAMPLING_RATE_IN_MICROSECONDS { 100U };
}

void on_exit()
{
    profiler.unloadAllModules(__rdtsc());
    profiler.stop();
    drsym_exit();

    dr_printf("%s\n", __func__);

    dr_printf("Module Records in CSV\n=====\n%s\n", profiler.getModuleRecordsAsCSV().c_str());
    dr_printf("Thread Records in CSV\n=====\n%s\n", profiler.getThreadRecordsAsCSV().c_str());
}

void find_prof_timer_func(const module_data_t *module)
{
#ifdef LINUX
    if (!module)
        return;

    auto func = dr_get_proc_address(module->handle, "setitimer");

    if (!func)
        return;

    enable_prof_timer = reinterpret_cast<Func>(func);

    //dr_printf("%s: Found 'setitimer' in %s at %p\n", __func__, module->full_path, func);
#endif
}

void call_prof_timer_func(const std::uint64_t interval)
{
#ifdef LINUX
    if (!enable_prof_timer)
        return;

    itimerval rate;
    rate.it_value.tv_sec = 0;
    rate.it_value.tv_usec = interval;
    rate.it_interval.tv_sec = 0;
    rate.it_interval.tv_usec = interval;
    enable_prof_timer(ITIMER_PROF, &rate, NULL);
#endif
}

bool on_symbol(const char *name, size_t modoffs, void *data)
{
    auto module = static_cast<module_data_t *>(data);
    CoMe::Symbol symbol { std::string(name), modoffs, std::string(module->full_path) };
    profiler.registerSymbol(symbol);
    return true;
}

void instrument_symbols_in_module(const module_data_t *data)
{
    if (!data)
        return;

    auto data_copy = dr_copy_module_data(data);
    drsym_enumerate_symbols(data->full_path, on_symbol, data_copy, DRSYM_DEFAULT_FLAGS);
    dr_free_module_data(data_copy);
}

void on_module_load(void *ctx, const module_data_t *data, bool loaded)
{
    if (!data || !data->full_path)
        return;

    auto err = profiler.loadModule(reinterpret_cast<std::uint64_t>(data->start), reinterpret_cast<std::uint64_t>(data->end), __rdtsc(), std::string(data->full_path));
    instrument_symbols_in_module(data);
    find_prof_timer_func(data);
    call_prof_timer_func(DEFAULT_SAMPLING_RATE_IN_MICROSECONDS);

    //dr_printf("%s: Context %p, Addr %p, Path %s, State %s at %llu, Added: %u, Total Amount %u\n",
    //    __func__, ctx, data->start, data->full_path, loaded ? "LOADED":"UNLOADED", module.LoadTSC, err, profiler.getLoadedModules().size());
}

void on_module_unload(void *ctx, const module_data_t *data)
{
    if (!data)
        return;

    const std::uint64_t UnloadTSC = __rdtsc();
    auto err = profiler.unloadModule(std::string(data->full_path), UnloadTSC);

    //dr_printf("%s: Context %p, Addr %p, Path %s, State %s at %llu, Removed: %u, Total Amount %u\n",
    //    __func__, ctx, data->start, data->full_path, "UNLOADED", UnloadTSC, err, profiler.getLoadedModules().size());

}

#ifdef WINDOWS
bool on_exception(void *ctx, dr_exception_t *e)
{
    if (e)
        dr_printf("%s: Context %p, Addr %p\n", __func__, ctx, e->fault_fragment_info.cache_start_pc);
    else
        dr_printf("%s: exception is NULL\n", __func__);

    return true;
}
#endif

#ifdef LINUX
dr_signal_action_t on_signal(void *ctx, dr_siginfo_t *siginfo)
{
    dr_signal_action_t action = DR_SIGNAL_DELIVER;

    if (!siginfo)
        return action;

    CoMe::Sample s {
        reinterpret_cast<std::uint64_t>(ctx),
        reinterpret_cast<std::uint64_t>(siginfo->mcontext->xsp),
        reinterpret_cast<std::uint64_t>(siginfo->mcontext->xbp),
        __rdtsc()
    };

    if (siginfo->sig == SIGPROF)
        action = DR_SIGNAL_SUPPRESS;

    profiler.recordSample(s);

    //dr_printf("%s: Context %p, DRContext %p, SP %p, BP %p, Sig %d\n", __func__, ctx,
    //    siginfo->drcontext, siginfo->mcontext->xsp, siginfo->mcontext->xbp, siginfo->sig);

    return action;
}
#endif

void on_thread_init(void *ctx)
{
    //dr_printf("%s: Thread %p started at %llu, timer_func_ptr %p \n", __func__, ctx, __rdtsc(), enable_prof_timer);

    profiler.startThread(reinterpret_cast<const std::uint64_t>(ctx), __rdtsc());
    call_prof_timer_func(DEFAULT_SAMPLING_RATE_IN_MICROSECONDS);
}

void on_thread_exit(void *ctx)
{
    //dr_printf("%s: Thread %p finished at %llu\n", __func__, ctx, __rdtsc());

    profiler.finishThread(reinterpret_cast<const std::uint64_t>(ctx), __rdtsc());
}

dr_emit_flags_t on_basicblock(void *ctx, void *tag, instrlist_t *bb, bool for_trace, bool translating)
{
    dr_emit_flags_t emit_flags = DR_EMIT_DEFAULT;
/*
    auto instr = instrlist_first(bb);
    auto app_pc = instr_get_app_pc(instr);
    auto module = profiler.getModuleNameByAddress(reinterpret_cast<std::uint64_t>(app_pc));

    dr_printf("%s: Context %p, Tag %p, BB %p (APP PC %p), ForTrace: %d, Translating %d, Module: '%s' (%u)\n",
        __func__, ctx, tag, bb, app_pc, for_trace, translating, module.c_str(), profiler.getLoadedModules().size());
*/
    return emit_flags;
}

dr_emit_flags_t on_trace(void *ctx, void *tag, instrlist_t *trace, bool translating)
{
    dr_emit_flags_t emit_flags = DR_EMIT_DEFAULT;

    //dr_printf("%s: Context %p, Tag %p, Trace %p, Translating %d\n", __func__, ctx, tag, trace, translating);

    return emit_flags;
}

void enable_logging()
{
    // Need to enable console logging specifically on Windows
#ifdef WINDOWS
    dr_enable_console_printing();
#endif
}

void print_application_arguments(client_id_t id, const int argc, const char **argv)
{
    for (int i = 0; i < argc; ++i)
        dr_printf("%s: ClientID %u, argv[%d] %s\n", __func__, id, i, argv[i]);
}

void register_module_profiling_callbacks()
{
    dr_register_module_load_event(on_module_load);
    dr_register_module_unload_event(on_module_unload);
}

void register_thread_profiling_callbacks()
{
    dr_register_thread_init_event(on_thread_init);
    dr_register_thread_exit_event(on_thread_exit);
}

void register_runtime_error_callbacks()
{
#ifdef WINDOWS
    dr_register_exception_event(on_exception);
#endif
#ifdef LINUX
    dr_register_signal_event(on_signal);
#endif
}

void register_profiling_callbacks()
{
    register_module_profiling_callbacks();
    register_thread_profiling_callbacks();
    //dr_register_bb_event(on_basicblock);
    //dr_register_trace_event(on_trace);
    register_runtime_error_callbacks();
    dr_register_exit_event(on_exit);
}

bool init()
{
    if (DRSYM_SUCCESS != drsym_init(0))
        return false;

    if (!profiler.start())
        return false;

    return true;
}

DR_EXPORT
void dr_client_main(client_id_t	id, int	argc, const char **argv)
{
    if (!init())
        return;

    print_application_arguments(id, argc, argv);
    register_profiling_callbacks();
}



