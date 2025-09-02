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

#include "profiler.hpp"
#include "basicblock.hpp"
#include "symbol.hpp"
#include "thread.hpp"

#include <gtest/gtest.h>

namespace CoMe {
namespace Test {
namespace {

class ProfilerTest : public testing::Test {
protected:
    ProfilerTest() {}
    ~ProfilerTest() override {}
    void SetUp() override {}
    void TearDown() override {}

    auto createTestModules(unsigned amount)
    {
        Profiler::Modules container;
        for (unsigned i = 0; i < amount; i++)
            container.emplace_back(Module(i + 1000U, i + 1000U + 100U, 10U, 100U, std::string("mod") + std::to_string(i)));
        return container;
    }

    auto createTestThreads(unsigned amount)
    {
        Profiler::Threads container;
        for (unsigned i = 0; i < amount; i++)
            container.emplace_back(Thread(i + 1000U, i + 1100U, i));
        return container;
    }

    auto createTestSamples(unsigned amount)
    {
        Profiler::SamplesContainer container;
        for (unsigned i = 0; i < amount; i++)
            container.emplace_back(Sample(i, i + 1000U, i + 2000U, i));
        return container;
    }

    const std::uint64_t getInnerAddress(const std::uint64_t start, const std::uint64_t end)
    {
        return (start + end) / 2;
    }
};

TEST_F(ProfilerTest, AllModulesReportedDuringActiveProfiling) {
    const auto testModules = createTestModules(2);
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(testModules[0].StartAddress, testModules[0].EndAddress, testModules[0].LoadTSC, testModules[0].FullPath), true);
    EXPECT_EQ(p.loadModule(testModules[1].StartAddress, testModules[1].EndAddress, testModules[1].LoadTSC, testModules[1].FullPath), true);
    EXPECT_EQ(p.stop(), true);
    auto modules = p.getLoadedModules();
    EXPECT_EQ(modules.size(), testModules.size());
    for (std::size_t i = 0U; i < modules.size(); i++)
    {
        EXPECT_EQ(modules[i].StartAddress, testModules[i].StartAddress);
        EXPECT_EQ(modules[i].EndAddress, testModules[i].EndAddress);
        EXPECT_EQ(modules[i].LoadTSC, testModules[i].LoadTSC);
        EXPECT_EQ(modules[i].UnloadTSC, 0U);
        EXPECT_EQ(modules[i].FullPath, testModules[i].FullPath);
    }
}

TEST_F(ProfilerTest, SomeModulesReportedBeforeAndAfterActiveProfiling) {
    Profiler p;
    const auto testModules = createTestModules(4);
    EXPECT_EQ(p.loadModule(testModules[0].StartAddress, testModules[0].EndAddress, testModules[0].LoadTSC, testModules[0].FullPath), false);
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(testModules[1].StartAddress, testModules[1].EndAddress, testModules[1].LoadTSC, testModules[1].FullPath), true);
    EXPECT_EQ(p.loadModule(testModules[2].StartAddress, testModules[2].EndAddress, testModules[2].LoadTSC, testModules[2].FullPath), true);
    EXPECT_EQ(p.stop(), true);
    EXPECT_EQ(p.loadModule(testModules[3].StartAddress, testModules[3].EndAddress, testModules[3].LoadTSC, testModules[3].FullPath), false);
    auto modules = p.getLoadedModules();
    EXPECT_EQ(modules.size(), 2);
    EXPECT_EQ(modules[0].StartAddress, testModules[1].StartAddress);
    EXPECT_EQ(modules[0].EndAddress  , testModules[1].EndAddress);
    EXPECT_EQ(modules[0].LoadTSC     , testModules[1].LoadTSC);
    EXPECT_EQ(modules[0].UnloadTSC   , 0U);
    EXPECT_EQ(modules[0].FullPath    , testModules[1].FullPath);
    EXPECT_EQ(modules[1].StartAddress, testModules[2].StartAddress);
    EXPECT_EQ(modules[1].EndAddress  , testModules[2].EndAddress);
    EXPECT_EQ(modules[1].LoadTSC     , testModules[2].LoadTSC);
    EXPECT_EQ(modules[1].UnloadTSC   , 0U);
    EXPECT_EQ(modules[1].FullPath    , testModules[2].FullPath);
}

TEST_F(ProfilerTest, UnloadModule) {
    Profiler p;
    const auto testModules = createTestModules(4);
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(testModules[0].StartAddress, testModules[0].EndAddress, testModules[0].LoadTSC, testModules[0].FullPath), true);
    EXPECT_EQ(p.unloadModule(testModules[0].FullPath, 100U), true);
    EXPECT_EQ(p.stop(), true);
    EXPECT_EQ(p.getLoadedModules().size(), 0);
}

TEST_F(ProfilerTest, UnloadModuleAllModules) {
    const auto testModules = createTestModules(2);
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(testModules[0].StartAddress, testModules[0].EndAddress, testModules[0].LoadTSC, testModules[0].FullPath), true);
    EXPECT_EQ(p.loadModule(testModules[1].StartAddress, testModules[1].EndAddress, testModules[1].LoadTSC, testModules[1].FullPath), true);
    p.unloadAllModules(100U);
    EXPECT_EQ(p.stop(), true);
    auto modules = p.getLoadedModules();
    EXPECT_EQ(modules.size(), 0);
}

TEST_F(ProfilerTest, GetModuleNameByStartAddress) {
    const auto testModules = createTestModules(1);
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(testModules[0].StartAddress, testModules[0].EndAddress, testModules[0].LoadTSC, testModules[0].FullPath), true);
    EXPECT_EQ(p.stop(), true);
    auto name = p.getLoadedModuleNameByAddress(testModules[0].StartAddress);
    EXPECT_EQ(name, std::string("mod0"));
}

TEST_F(ProfilerTest, GetModuleNameByEndAddress) {
    const auto testModules = createTestModules(1);
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(testModules[0].StartAddress, testModules[0].EndAddress, testModules[0].LoadTSC, testModules[0].FullPath), true);
    EXPECT_EQ(p.stop(), true);
    auto name = p.getLoadedModuleNameByAddress(testModules[0].EndAddress);
    EXPECT_EQ(name, std::string("mod0"));
}

TEST_F(ProfilerTest, GetModuleNameByInnerAddress) {
    const auto testModules = createTestModules(1);
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(testModules[0].StartAddress, testModules[0].EndAddress, testModules[0].LoadTSC, testModules[0].FullPath), true);
    EXPECT_EQ(p.stop(), true);
    auto innerAddress = getInnerAddress(testModules[0].StartAddress, testModules[0].EndAddress);
    auto name = p.getLoadedModuleNameByAddress(innerAddress);
    EXPECT_EQ(name, std::string("mod0"));
}

TEST_F(ProfilerTest, RegisterModuleSymbol) {
    Profiler p;
    Symbol s;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.registerSymbol(s), true);
    EXPECT_EQ(p.stop(), true);
}

TEST_F(ProfilerTest, GetModuleSymbolByName) {
    Profiler p;
    Symbol s { std::string("sym0"), 42U, std::string("mod0") };
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.registerSymbol(s), true);
    EXPECT_EQ(p.stop(), true);
    const Symbol &symbol = p.getSymbolByName(std::string("sym0"), std::string("mod0"));
    EXPECT_EQ(symbol.Name, std::string("sym0"));
    EXPECT_EQ(symbol.Address, 42U);
    EXPECT_EQ(symbol.Module, std::string("mod0"));
}

TEST_F(ProfilerTest, StartFinishThread) {
    Profiler p;
    auto threads = createTestThreads(1);
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.startThread(threads[0].Context, threads[0].StartTSC), true);
    EXPECT_EQ(p.finishThread(threads[0].Context, threads[0].FinishTSC), true);
    EXPECT_EQ(p.stop(), true);
}

TEST_F(ProfilerTest, GetAllStartedThreads) {
    Profiler p;
    auto threads = createTestThreads(4);
    EXPECT_EQ(p.startThread(threads[0].Context, threads[0].StartTSC), false);
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.startThread(threads[1].Context, threads[1].StartTSC), true);
    EXPECT_EQ(p.startThread(threads[2].Context, threads[2].StartTSC), true);
    EXPECT_EQ(p.stop(), true);
    EXPECT_EQ(p.startThread(threads[3].Context, threads[3].StartTSC), false);
    auto startedThreads = p.getStartedThreads();
    EXPECT_EQ(startedThreads.size(), 2);
    EXPECT_EQ(startedThreads[0].StartTSC , threads[1].StartTSC);
    EXPECT_EQ(startedThreads[0].FinishTSC, 0U);
    EXPECT_EQ(startedThreads[0].Context  , threads[1].Context);
    EXPECT_EQ(startedThreads[1].StartTSC , threads[2].StartTSC);
    EXPECT_EQ(startedThreads[1].FinishTSC, 0U);
    EXPECT_EQ(startedThreads[1].Context  , threads[2].Context);
}

TEST_F(ProfilerTest, ReportSample) {
    Profiler p;
    auto samples = createTestSamples(4);
    EXPECT_EQ(p.recordSample(samples[0]), false);
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.recordSample(samples[1]), true);
    EXPECT_EQ(p.recordSample(samples[2]), true);
    EXPECT_EQ(p.stop(), true);
    EXPECT_EQ(p.recordSample(samples[3]), false);
    auto recordedSamples = p.getRecordedSamples();
    EXPECT_EQ(recordedSamples.size(), 2);
    EXPECT_EQ(recordedSamples[0] == samples[1], true);
    EXPECT_EQ(recordedSamples[1] == samples[2], true);
}

TEST_F(ProfilerTest, GetModuleRecordsFormattedInCSV) {
    Profiler p;
    auto modules = createTestModules(10);
    EXPECT_EQ(p.start(), true);
    for (const auto &module : modules)
    {
        EXPECT_EQ(p.loadModule(module.StartAddress, module.EndAddress, module.LoadTSC, module.FullPath), true);
        EXPECT_EQ(p.unloadModule(module.FullPath, module.UnloadTSC), true);
    }
    EXPECT_EQ(p.stop(), true);
    auto moduleRecordsCSV = p.getModuleRecordsAsCSV();
    EXPECT_EQ(moduleRecordsCSV.empty(), false);
}

TEST_F(ProfilerTest, GetThreadRecordsFormattedInCSV) {
    Profiler p;
    auto threads = createTestThreads(10);
    EXPECT_EQ(p.start(), true);
    for (const auto &thread : threads)
    {
        EXPECT_EQ(p.startThread(thread.Context, thread.StartTSC), true);
        EXPECT_EQ(p.finishThread(thread.Context, thread.FinishTSC), true);
    }
    EXPECT_EQ(p.stop(), true);
    auto threadRecordsCSV = p.getThreadRecordsAsCSV();
    EXPECT_EQ(threadRecordsCSV.empty(), false);
}

}  // namespace
}  // namespace Test
}  // namespace CoMe

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}