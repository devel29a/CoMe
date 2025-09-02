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

#include "profiler.hpp"

#include <algorithm>
#include <iostream>

namespace
{

auto findLoadedModuleByAddress(const CoMe::Profiler::Modules &modules, const std::uint64_t address)
{
    return std::find_if(modules.cbegin(), modules.cend(),
        [&](const auto &m)
        {
            return m.StartAddress <= address && address <= m.EndAddress;
        });
}

auto findLoadedModuleByName(const CoMe::Profiler::Modules &modules, const std::string &module)
{
    return std::find_if(modules.begin(), modules.end(),
        [&](const auto &m)
        {
            return m.FullPath == module;
        });
}

auto findStartedThread(const CoMe::Profiler::Threads &threads, const std::uint64_t context)
{
    return std::find_if(threads.begin(), threads.end(),
        [&](const auto &t)
        {
            return t.Context == context;
        });
}

bool isModuleLoaded(const CoMe::Profiler::Modules &modules, CoMe::Profiler::Modules::const_iterator iter)
{
    return iter != modules.cend();
}

bool isThreadStarted(const CoMe::Profiler::Threads &threads, CoMe::Profiler::Threads::const_iterator iter)
{
    return iter != threads.cend();
}

auto findRegisterSymbolByName(const CoMe::Profiler::SymbolsContainer &symbols, const std::string &name, const std::string &module)
{
    return std::find_if(symbols.cbegin(), symbols.cend(),
        [&](const auto s)
        {
            return s.Name == name && s.Module == module;
        });
}

bool isSymbolRegistered(const CoMe::Profiler::SymbolsContainer &symbols, CoMe::Profiler::SymbolsContainer::const_iterator iter)
{
    return iter != symbols.cend();
}
}

namespace CoMe
{
bool Profiler::start()
{
    isProfilingActive = true;
    return true;
}
bool Profiler::stop()
{
    isProfilingActive = false;
    return true;
}

const Profiler::Modules& Profiler::getLoadedModules() const
{
    return loadedModules;
}

const Profiler::SymbolsContainer& Profiler::getRegisteredSymbols() const
{
    return RegisteredSymbols;
}

const Profiler::Threads& Profiler::getStartedThreads() const
{
    return startedThreads;
}

const Profiler::SamplesContainer& Profiler::getRecordedSamples() const
{
    return RecordedSamples;
}

bool Profiler::loadModule(const Module &module)
{
    if (!isProfilingActive)
        return false;

    loadedModules.emplace_back(Module(module.StartAddress, module.EndAddress, module.LoadTSC, 0U, module.FullPath));
    return true;
}

bool Profiler::unloadModule(const std::string &module, const std::uint64_t unloadTSC)
{
    if (!isProfilingActive)
        return false;

    auto it = findLoadedModuleByName(this->getLoadedModules(), module);

    if (!isModuleLoaded(this->getLoadedModules(), it))
        return false;

    const_cast<Module&>(*it).UnloadTSC = unloadTSC;
    ledger.recordModule(*it);

    loadedModules.erase(it);
    return true;
}

void Profiler::unloadAllModules(const std::uint64_t unloadTSC)
{
    for (const auto module : loadedModules)
        ledger.recordModule(module);

    loadedModules.clear();
}

const std::string& Profiler::getModuleNameByAddress(const std::uint64_t address)
{
    static const std::string EmptyPath;
    auto it = findLoadedModuleByAddress(this->getLoadedModules(), address);

    if (!isModuleLoaded(this->getLoadedModules(), it))
        return EmptyPath;

    return it->FullPath;
}

bool Profiler::registerSymbol(const Symbol &symbol)
{
    if (!isProfilingActive)
        return false;

    RegisteredSymbols.push_back(symbol);
    return true;
}

const Symbol& Profiler::getSymbolByName(const std::string &symbol, const std::string &module)
{
    static const Symbol EmptySymbol;
    const auto it = findRegisterSymbolByName(this->getRegisteredSymbols(), symbol, module);

    if (!isSymbolRegistered(this->getRegisteredSymbols(), it))
        return EmptySymbol;

    return *it;
}

bool Profiler::startThread(const Thread &thread)
{
    if (!isProfilingActive)
        return false;

    startedThreads.push_back(thread);
    return true;
}

bool Profiler::finishThread(const std::uint64_t context, std::uint64_t finishTSC)
{
    if (!isProfilingActive)
        return false;

    auto it = findStartedThread(this->getStartedThreads(), context);

    if (!isThreadStarted(this->getStartedThreads(), it))
        return false;

    const_cast<Thread&>(*it).FinishTSC = finishTSC;
    ledger.recordThread(*it);

    startedThreads.erase(it);
    return true;
}

bool Profiler::recordSample(const Sample &sample)
{
    if (!isProfilingActive)
        return false;

    RecordedSamples.push_back(sample);
    return true;
}

}
