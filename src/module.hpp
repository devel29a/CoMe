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

#include <cstdint>
#include <string>
#include <vector>

namespace CoMe
{
namespace Module
{    

struct Instance
{
    Instance(
        std::uint64_t StartAddr_,
        std::uint64_t EndAddr_,
        std::uint64_t LoadTSC_,
        std::uint64_t UnloadTSC_,
        const char * FullPath_)
    {
        StartAddr = StartAddr_;
        EndAddr = EndAddr_;
        LoadTSC = LoadTSC_;
        UnloadTSC = UnloadTSC_;
        FullPath = FullPath_;
    }

    std::uint64_t StartAddr { 0 };
    std::uint64_t EndAddr { 0 };
    std::uint64_t LoadTSC { 0 };
    std::uint64_t UnloadTSC { 0 };
    std::string FullPath;
};

using Cache = std::vector<Module::Instance>;

} // Module
} // CoMe
