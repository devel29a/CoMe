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

#include "ledger.hpp"

namespace
{
    bool isModuleRecordValid(const CoMe::Module &module)
    {
        return module.StartAddress > 0                 &&
               module.StartAddress < module.EndAddress &&
               module.LoadTSC      > 0                 &&
               module.LoadTSC      < module.UnloadTSC  &&
               !module.FullPath.empty()                  ;
    }

    bool isThreadRecordValid(const CoMe::Thread &thread)
    {
        return thread.Context && thread.StartTSC &&
               thread.StartTSC < thread.FinishTSC  ;
    }

    bool isSampleRecordValid(const CoMe::Sample &sample)
    {
        return sample.BP && sample.SP && sample.TSC;
    }
}

namespace CoMe
{
    bool Ledger::recordModule(const Module &module)
    {
        if (!isModuleRecordValid(module))
            return false;

        moduleRecords.push_back(module);
        return true;
    }

    bool Ledger::recordThread(const Thread &thread)
    {
        if (!isThreadRecordValid(thread))
            return false;

        threadRecords.push_back(thread);
        return true;
    }

    bool Ledger::recordSample(const std::uint64_t thread, const Sample &sample)
    {
        if (!isSampleRecordValid(sample))
            return false;

        sampleRecords[thread].push_back(sample);
        return true;
    }

    const Ledger::ModuleRecords& Ledger::getModuleRecords() const
    {
        return moduleRecords;
    }

    const Ledger::ThreadRecords& Ledger::getThreadRecords() const
    {
        return threadRecords;
    }

    const Ledger::SampleRecords& Ledger::getSampleRecords() const
    {
        return sampleRecords;
    }
}