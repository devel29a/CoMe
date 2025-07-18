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

auto findLoadedModuleByAddress(const CoMe::Profiler::ModulesContainer &modules, const std::uint64_t address)
{
    return std::find_if(modules.cbegin(), modules.cend(),
        [&](const auto &m)
        {
            return m.StartAddress <= address && address <= m.EndAddress;
        });
}

auto findLoadedModuleByName(const CoMe::Profiler::ModulesContainer &modules, const std::string &module)
{
    return std::find_if(modules.cbegin(), modules.cend(),
        [&](const auto &m)
        {
            return m.FullPath == module;
        });
}

bool isModuleLoaded(const CoMe::Profiler::ModulesContainer &modules, CoMe::Profiler::ModulesContainer::const_iterator iter)
{
    return iter != modules.cend();
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

const Profiler::ModulesContainer& Profiler::getLoadedModules()
{
    return LoadedModules;
}

const Profiler::SymbolsContainer& Profiler::getRegisteredSymbols()
{
    return RegisteredSymbols;
}

const Profiler::ThreadsContainer& Profiler::getStartedThreads()
{
    return StartedThreads;
}

bool Profiler::loadModule(const Module &module)
{
    if (!isProfilingActive)
        return false;

    LoadedModules.emplace_back(Module(module.StartAddress, module.EndAddress, module.LoadTSC, 0U, module.FullPath));
    return true;
}

bool Profiler::unloadModule(const std::string &module)
{
    if (!isProfilingActive)
        return false;

    auto it = findLoadedModuleByName(this->getLoadedModules(), module);

    if (!isModuleLoaded(this->getLoadedModules(), it))
        return false;

    LoadedModules.erase(it);
    return true;
}

void Profiler::unloadAllModules(const std::uint64_t unloadTSC)
{
    LoadedModules.clear();
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

    StartedThreads.push_back(thread);
    return true;
}

bool Profiler::finishThread(const Thread &thread)
{
    if (!isProfilingActive)
        return false;

    auto it = std::remove(StartedThreads.begin(), StartedThreads.end(), thread);

    if (StartedThreads.cend() == it)
        return false;

    StartedThreads.erase(it);
    return true;
}
}
