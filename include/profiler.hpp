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
#include "sample.hpp"
#include "ledger.hpp"

namespace CoMe
{
class Profiler
{
public:
    using Modules = std::vector<Module>;
    using SymbolsContainer = std::vector<Symbol>;
    using Threads = std::vector<Thread>;
    using SamplesContainer = std::vector<Sample>;

    bool start();
    bool stop();
    const Modules& getLoadedModules() const;
    const SymbolsContainer& getRegisteredSymbols() const;
    const Threads& getStartedThreads() const;
    const SamplesContainer& getRecordedSamples() const;
    bool loadModule(const std::uint64_t startAddress, const std::uint64_t endAddress, const std::uint64_t loadTSC, const std::string &module);
    bool unloadModule(const std::string &module, const std::uint64_t unloadTSC);
    void unloadAllModules(const std::uint64_t unloadTSC);
    const std::string& getLoadedModuleNameByAddress(const std::uint64_t address);
    bool registerSymbol(const Symbol &symbol);
    const Symbol& getSymbolByName(const std::string &symbol, const std::string &module);
    bool startThread(const std::uint64_t context, std::uint64_t startTSC);
    bool finishThread(const std::uint64_t context, std::uint64_t finishTSC);
    bool recordSample(const Sample &sample);

    const std::string getModuleRecordsAsCSV() const;
    const std::string getThreadRecordsAsCSV() const;

private:
    Modules loadedModules;
    SymbolsContainer RegisteredSymbols;
    Threads startedThreads;
    SamplesContainer RecordedSamples;
    bool isProfilingActive { false };
    Ledger ledger;
};
}