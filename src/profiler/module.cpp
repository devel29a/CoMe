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

#include "module.hpp"

namespace CoMe
{
Module::Module(){}

Module::Module(const std::uint64_t StartAddress,
               const std::uint64_t EndAddress,
               const std::uint64_t LoadTSC,
               const std::uint64_t UnloadTSC,
               const std::string &FullPath)
{
    this->StartAddress = StartAddress;
    this->EndAddress = EndAddress;
    this->LoadTSC = LoadTSC;
    this->UnloadTSC = UnloadTSC;
    this->FullPath = FullPath;
}

const Module& Module::operator=(const Module& module)
{
    this->StartAddress = module.StartAddress;
    this->EndAddress   = module.EndAddress;
    this->LoadTSC      = module.LoadTSC;
    this->UnloadTSC    = module.UnloadTSC;
    this->FullPath     = module.FullPath;

    return *this;
}

bool Module::operator==(const CoMe::Module& module) const
{
    return this->StartAddress == module.StartAddress &&
           this->EndAddress   == module.EndAddress   &&
           this->LoadTSC      == module.LoadTSC      &&
           this->UnloadTSC    == module.UnloadTSC    &&
           this->FullPath     == module.FullPath       ;
}
}