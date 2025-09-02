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

#include "formatter.hpp"

#include <sstream>

#include "module.hpp"

namespace
{
    const std::string ToCSV(const CoMe::Module &module)
    {
        std::string csv;
        csv += std::to_string(module.StartAddress) + ",";
        csv += std::to_string(module.EndAddress  ) + ",";
        csv += std::to_string(module.LoadTSC     ) + ",";
        csv += std::to_string(module.UnloadTSC   ) + ",";
        csv +=                module.FullPath           ;
        return csv;
    }
}

namespace CoMe
{
namespace Formatter
{
const std::string ToCSV(const Ledger::ModuleRecords &records)
{
    std::stringstream csv;
    for (const auto &record : records)
        csv << ::ToCSV(record) << '\n';
    csv.str().pop_back();
    return csv.str();
}
}
}
