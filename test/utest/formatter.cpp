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
#include "ledger.hpp"

#include <gtest/gtest.h>

namespace CoMe {
namespace Test {
namespace {

class FormatterTest : public testing::Test {
protected:
    FormatterTest() {}
    ~FormatterTest() override {}
    void SetUp() override {}
    void TearDown() override {}

    const auto createTestModuleRecords(unsigned amount)
    {
        Ledger::ModuleRecords records;
        for (unsigned i = 0; i < amount; i++)
            records.emplace_back(Module(i * 1000U + 1000U, i * 1000U + 1500U, i * 10U + 10U, 1000U, std::string("mod") + std::to_string(i)));
        return records;
    }

    const auto formatCSVTestModuleRecord(const Module &module)
    {
        std::string csv;
        csv += std::to_string(module.StartAddress) + ",";
        csv += std::to_string(module.EndAddress  ) + ",";
        csv += std::to_string(module.LoadTSC     ) + ",";
        csv += std::to_string(module.UnloadTSC   ) + ",";
        csv +=                module.FullPath           ;
        return csv;
    }
};

TEST_F(FormatterTest, FormatModuleObjectAsCSV) {
    Ledger l;
    auto moduleRecords = createTestModuleRecords(2);
    for (const auto &record : moduleRecords)
        l.recordModule(record);
    auto text = Formatter::ToCSV(l.getModuleRecords());
    EXPECT_EQ(text,
              formatCSVTestModuleRecord(moduleRecords[0]) + std::string("\n") +
              formatCSVTestModuleRecord(moduleRecords[1]) + std::string("\n")  );
}

}  // namespace
}  // namespace Test
}  // namespace CoMe

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}