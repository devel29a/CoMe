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

void on_exit()
{
    dr_printf("%s: called\n", __func__);
}

void on_module_load(void *ctx, const module_data_t *data, bool loaded)
{
    if (data)
    {
        dr_printf("%s: Context %p, Addr %p, Path %s, State %s\n",
            __func__, ctx, data->start, data->full_path, loaded ? "LOADED":"UNLOADED");
    }
    else
    {
        dr_printf("%s: data is NULL\n", __func__);
    }
}

void on_module_unload(void *ctx, const module_data_t *data)
{
    if (data)
    {
        dr_printf("%s: Context %p, Addr %p, Path %s, State %s\n",
            __func__, ctx, data->start, data->full_path, "UNLOADED");
    }
    else
    {
        dr_printf("%s: data is NULL\n", __func__);
    }
}

#ifdef WINDOWS
bool on_exception(void *ctx, dr_exception_t *e)
{
    if (e)
    {
        dr_printf("%s: Context %p, Addr %p\n",
            __func__, ctx, e->fault_fragment_info.cache_start_pc);
    }
    else
    {
        dr_printf("%s: exception is NULL\n", __func__);
    }

    return true;
}
#endif

#ifdef LINUX
dr_signal_action_t on_signal(void *ctx, dr_siginfo_t *siginfo)
{
    dr_signal_action_t action = DR_SIGNAL_DELIVER;

    if (siginfo)
    {
        dr_printf("%s: Context %p, Addr %p, Sig %d\n",
            __func__, ctx, siginfo->fault_fragment_info.cache_start_pc, siginfo->sig);
    }
    else
    {
        dr_printf("%s: siginfo is NULL\n", __func__);
    }

    return action;
}
#endif

void on_thread_init(void *ctx)
{
    dr_printf("%s: called\n", __func__);
}

void on_thread_exit(void *ctx)
{
    dr_printf("%s: called\n", __func__);
}

dr_emit_flags_t on_basicblock(void *ctx, void *tag, instrlist_t *bb, bool for_trace, bool translating)
{
    dr_emit_flags_t emit_flags = DR_EMIT_DEFAULT;

    dr_printf("%s: Context %p, Tag %p, BB %p, ForTrace: %d, Translating %d\n",
        __func__, ctx, tag, bb, for_trace, translating);

    return emit_flags;
}

dr_emit_flags_t on_trace(void *ctx, void *tag, instrlist_t *trace, bool translating)
{
    dr_emit_flags_t emit_flags = DR_EMIT_DEFAULT;

    dr_printf("%s: Context %p, Tag %p, Trace %p, Translating %d\n",
        __func__, ctx, tag, trace, translating);

    return emit_flags;
}

DR_EXPORT
void dr_client_main(client_id_t	id, int	argc, const char **argv)
{
    // Need to enable console logging specifically on Windows
#ifdef WINDOWS
    dr_enable_console_printing();
#endif

    // Show input data
    for (int i = 0; i < argc; ++i)
    {
        dr_printf("%s: ClientID %u, argv[%d] %s\n", __func__, id, i, argv[i]);
    }

    // Registers a callback function for the module load event
    dr_register_module_load_event(on_module_load);

    // Registers a callback function for the module unload event
    dr_register_module_unload_event(on_module_unload);

    // Registers a callback function for the thread initialization event
    dr_register_thread_init_event(on_thread_init);

    // Registers a callback function for the thread exit event
    dr_register_thread_exit_event(on_thread_exit);

    // Registers a callback function for the basic block event
    dr_register_bb_event(on_basicblock);

    // Registers a callback function for the trace event
    dr_register_trace_event(on_trace);

    // Registers a callback for the process exit event
    dr_register_exit_event(on_exit);

#ifdef WINDOWS
    // Registers a callback function for the exception event
    dr_register_exception_event(on_exception);
#endif

#ifdef LINUX
    // Registers a callback function for the signal event
    dr_register_signal_event(on_signal);
#endif
}



