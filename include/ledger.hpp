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

#include <cstdint>
#include <vector>
#include <map>

#include "module.hpp"
#include "thread.hpp"
#include "sample.hpp"

namespace CoMe
{
class Ledger
{
public:
    using ModuleRecords = std::vector<Module>;
    using ThreadRecords = std::vector<Thread>;
    using SampleRecords = std::map<std::uint64_t, std::vector<Sample>>;

    bool recordModule(const Module &module);
    bool recordThread(const Thread &thread);
    bool recordSample(const std::uint64_t thread, const Sample &sample);

    const ModuleRecords& getModuleRecords() const;
    const ThreadRecords& getThreadRecords() const;
    const SampleRecords& getSampleRecords() const;

private:
    ModuleRecords moduleRecords;
    ThreadRecords threadRecords;
    SampleRecords sampleRecords;
};
} // namespace CoMe
