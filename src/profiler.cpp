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

bool Profiler::loadModule(const Module &module)
{
    if (!isProfilingActive)
        return false;

    LoadedModules.push_back(module);
    return true;
}

bool Profiler::unloadModule(const Module &module)
{
    if (!isProfilingActive)
        return false;

    LoadedModules.erase(std::remove(LoadedModules.begin(), LoadedModules.end(), module));
    return true;
}

void Profiler::unloadAllModules(const std::uint64_t unloadTSC)
{
    LoadedModules.clear();
}

const std::string& Profiler::getModuleNameByAddress(const std::uint64_t address)
{
    static const std::string EmptyPath;

    auto it = std::find_if(LoadedModules.cbegin(), LoadedModules.cend(),
    [&](decltype(LoadedModules)::const_reference cref)
    {
        return cref.StartAddress <= address && address <= cref.EndAddress;
    });

    if (LoadedModules.end() == it)
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

    const auto s = std::find_if(RegisteredSymbols.cbegin(), RegisteredSymbols.cend(),
        [&](decltype(RegisteredSymbols)::const_reference cref)
        {
            return cref.Name == symbol && cref.Module == module;
        }
    );

    if (s != RegisteredSymbols.end())
        return *s;

    return EmptySymbol;
}

}
