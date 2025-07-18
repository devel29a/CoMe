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

#pragma once

#include <string>
#include <vector>

#include "module.hpp"
#include "symbol.hpp"
#include "thread.hpp"

namespace CoMe
{
class Profiler
{
public:
    using ModulesContainer = std::vector<Module>;
    using SymbolsContainer = std::vector<Symbol>;
    using ThreadsContainer = std::vector<Thread>;

    bool start();
    bool stop();
    const ModulesContainer& getLoadedModules();
    const SymbolsContainer& getRegisteredSymbols();
    const ThreadsContainer& getStartedThreads();
    bool loadModule(const Module &module);
    bool unloadModule(const std::string &module);
    void unloadAllModules(const std::uint64_t unloadTSC);
    const std::string& getModuleNameByAddress(const std::uint64_t address);
    bool registerSymbol(const Symbol &symbol);
    const Symbol& getSymbolByName(const std::string &symbol, const std::string &module);
    bool startThread(const Thread &thread);
    bool finishThread(const Thread &thread);

private:
    ModulesContainer LoadedModules;
    SymbolsContainer RegisteredSymbols;
    ThreadsContainer StartedThreads;
    bool isProfilingActive { false };
};
}