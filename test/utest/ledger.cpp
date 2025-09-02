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

#include <gtest/gtest.h>

namespace CoMe {
namespace Test {
namespace {

class LedgerTest : public testing::Test {
protected:
    LedgerTest() {}
    ~LedgerTest() override {}
    void SetUp() override {}
    void TearDown() override {}

    auto createTestModuleRecords(unsigned amount)
    {
        Ledger::ModuleRecords records;
        for (unsigned i = 0; i < amount; i++)
            records.emplace_back(Module(i * 1000U + 1000U, i * 1000U + 1500U, i * 10U + 10U, 1000U, std::string("mod") + std::to_string(i)));
        return records;
    }

    auto createTestThreadRecords(unsigned amount)
    {
        Ledger::ThreadRecords records;
        for (unsigned i = 0; i < amount; i++)
            records.emplace_back(Thread(i * 10U + 10U, i * 10U + 100U, i + 1));
        return records;
    }

    auto createTestSampleRecords(unsigned threadsAmount, unsigned samplesAmount)
    {
        Ledger::SampleRecords records;
        for (unsigned i = 0; i < threadsAmount; i++)
            for (unsigned j = 0; j < samplesAmount; j++)
                records[i + 10].emplace_back(Sample(i + 100U, i * 10U + 10U, i * 10U + 100U));
        return records;
    }
};

TEST_F(LedgerTest, RecordTenModules) {
    Ledger l;
    const auto records = createTestModuleRecords(10);
    for (const auto &r : records)
        EXPECT_EQ(l.recordModule(r), true);
}

TEST_F(LedgerTest, RecordModuleWithInvalidStartAddress) {
    Ledger l;
    const Module m { 0U, 1000U, 10U, 100U, std::string("mod") };
    EXPECT_EQ(l.recordModule(m), false);
}

TEST_F(LedgerTest, RecordModuleWithInvalidEndAddress) {
    Ledger l;
    const Module m { 10U, 0U, 10U, 100U, std::string("mod") };
    EXPECT_EQ(l.recordModule(m), false);
}

TEST_F(LedgerTest, RecordModuleWithInvalidLoadTSC) {
    Ledger l;
    const Module m { 10U, 100U, 0U, 100U, std::string("mod") };
    EXPECT_EQ(l.recordModule(m), false);
}

TEST_F(LedgerTest, RecordModuleWithInvalidUnloadTSC) {
    Ledger l;
    const Module m { 10U, 100U, 100U, 0U, std::string("mod") };
    EXPECT_EQ(l.recordModule(m), false);
}

TEST_F(LedgerTest, GetModuleRecords) {
    Ledger l;
    const auto records = createTestModuleRecords(10);
    for (const auto &r : records)
        EXPECT_EQ(l.recordModule(r), true);
    const auto moduleRecords = l.getModuleRecords();
    EXPECT_EQ(moduleRecords.size(), 10);
    for (std::size_t i = 0; i < moduleRecords.size(); i++)
        EXPECT_EQ(moduleRecords[i] == records[i], true);
}

TEST_F(LedgerTest, RecordTenThreads) {
    Ledger l;
    const auto records = createTestThreadRecords(10);
    for (const auto &r : records)
        EXPECT_EQ(l.recordThread(r), true);
}

TEST_F(LedgerTest, RecordThreadWithInvalidStartTSC) {
    Ledger l;
    const Thread t { 0U, 100U, 1U };
    EXPECT_EQ(l.recordThread(t), false);
}

TEST_F(LedgerTest, RecordThreadWithInvalidFinishTSC) {
    Ledger l;
    const Thread t { 100U, 0U, 1U };
    EXPECT_EQ(l.recordThread(t), false);
}

TEST_F(LedgerTest, RecordThreadWithInvalidContext) {
    Ledger l;
    const Thread t { 100U, 200U, 0U };
    EXPECT_EQ(l.recordThread(t), false);
}

TEST_F(LedgerTest, GetThreadRecords) {
    Ledger l;
    const auto records = createTestThreadRecords(10);
    for (const auto &r : records)
        EXPECT_EQ(l.recordThread(r), true);
    const auto threadRecords = l.getThreadRecords();
    EXPECT_EQ(threadRecords.size(), 10);
    for (std::size_t i = 0; i < threadRecords.size(); i++)
        EXPECT_EQ(threadRecords[i] == records[i], true);
}

TEST_F(LedgerTest, RecordTenSamplesFromSingleThread) {
    Ledger l;
    const auto records = createTestSampleRecords(1, 10);
    for (const auto &r : records.begin()->second)
        EXPECT_EQ(l.recordSample(records.begin()->first, r), true);
    auto samples = l.getSampleRecords();
    EXPECT_EQ(samples.size(), 1);
    const auto &samplesOneThread = samples[10];
    EXPECT_EQ(samplesOneThread.size(), 10);
    for (std::size_t i = 0; i < 10; i++)
        EXPECT_EQ(samplesOneThread.at(i), records.cbegin()->second.at(i));
}

}  // namespace
}  // namespace Test
}  // namespace CoMe

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}