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

#include "symbol.hpp"

namespace CoMe
{
Symbol::Symbol(){}

Symbol::Symbol(const std::string &name, const std::uint64_t address, const std::string &module)
{
    this->Name = name;
    this->Address = address;
    this->Module = module;
}

Symbol::Symbol(const Symbol &symbol)
{
    this->operator=(symbol);
}

const Symbol& Symbol::operator=(const Symbol &symbol)
{
    this->Name = symbol.Name;
    this->Address = symbol.Address;
    this->Module = symbol.Module;

    return *this;
}

bool Symbol::operator==(const Symbol &symbol)
{
    return this->Name    == symbol.Name    &&
           this->Address == symbol.Address &&
           this->Module  == symbol.Module    ;
}

}