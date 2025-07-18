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

    Profiler::ModulesContainer createTestModules(unsigned amount)
    {
        Profiler::ModulesContainer container;
        for (unsigned i = 0; i < amount; i++)
            container.emplace_back(Module(0U * 1000, 100U, 0U, 0U, std::string("mod") + std::to_string(i)));
        return container;
    }

    Profiler::ThreadsContainer createTestThreads(unsigned amount)
    {
        Profiler::ThreadsContainer container;
        for (unsigned i = 0; i < amount; i++)
            container.emplace_back(Thread(0U * 1000, 100U, i));
        return container;
    }

    const std::uint64_t getInnerAddress(const std::uint64_t start, const std::uint64_t end)
    {
        return (start + end) / 2;
    }
};

TEST_F(ProfilerTest, StartStopProfiling) {
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.stop(), true);
}

TEST_F(ProfilerTest, AllModulesReportedDuringActiveProfiling) {
    const auto modulesContainer = createTestModules(2);
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(modulesContainer[0]), true);
    EXPECT_EQ(p.loadModule(modulesContainer[1]), true);
    EXPECT_EQ(p.stop(), true);
    auto modules = p.getLoadedModules();
    EXPECT_EQ(modules.size(), 2);
    EXPECT_EQ(modules[0] == modulesContainer[0], true);
    EXPECT_EQ(modules[1] == modulesContainer[1], true);
}

TEST_F(ProfilerTest, SomeModulesReportedBeforeAndAfterActiveProfiling) {
    Profiler p;
    const auto modulesContainer = createTestModules(4);
    EXPECT_EQ(p.loadModule(modulesContainer[0]), false);
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(modulesContainer[1]), true);
    EXPECT_EQ(p.loadModule(modulesContainer[2]), true);
    EXPECT_EQ(p.stop(), true);
    EXPECT_EQ(p.loadModule(modulesContainer[3]), false);
    auto modules = p.getLoadedModules();
    EXPECT_EQ(modules.size(), 2);
    EXPECT_EQ(modules[0] == modulesContainer[1], true);
    EXPECT_EQ(modules[1] == modulesContainer[2], true);
}

TEST_F(ProfilerTest, UnloadModule) {
    Profiler p;
    Module m {12U, 34U, 56U, 78U, std::string("mod1")};
    EXPECT_EQ(m.StartAddress, 12U);
    EXPECT_EQ(m.EndAddress,   34U);
    EXPECT_EQ(m.LoadTSC,      56U);
    EXPECT_EQ(m.UnloadTSC,    78U);
    EXPECT_EQ(m.FullPath,  std::string("mod1"));
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(m), true);
    EXPECT_EQ(p.unloadModule(m), true);
    EXPECT_EQ(p.stop(), true);
    EXPECT_EQ(p.getLoadedModules().size(), 0);
}

TEST_F(ProfilerTest, UnloadModuleAllModules) {
    const auto modulesContainer = createTestModules(2);
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(modulesContainer[0]), true);
    EXPECT_EQ(p.loadModule(modulesContainer[1]), true);
    p.unloadAllModules(42U);
    EXPECT_EQ(p.stop(), true);
    auto modules = p.getLoadedModules();
    EXPECT_EQ(modules.size(), 0);
}

TEST_F(ProfilerTest, GetModuleNameByStartAddress) {
    const auto modulesContainer = createTestModules(1);
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(modulesContainer[0]), true);
    EXPECT_EQ(p.stop(), true);
    auto name = p.getModuleNameByAddress(modulesContainer[0].StartAddress);
    EXPECT_EQ(name, std::string("mod0"));
}

TEST_F(ProfilerTest, GetModuleNameByEndAddress) {
    const auto modulesContainer = createTestModules(1);
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(modulesContainer[0]), true);
    EXPECT_EQ(p.stop(), true);
    auto name = p.getModuleNameByAddress(modulesContainer[0].EndAddress);
    EXPECT_EQ(name, std::string("mod0"));
}

TEST_F(ProfilerTest, GetModuleNameByInnerAddress) {
    const auto modulesContainer = createTestModules(1);
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.loadModule(modulesContainer[0]), true);
    EXPECT_EQ(p.stop(), true);
    auto innerAddress = getInnerAddress(modulesContainer[0].StartAddress, modulesContainer[0].EndAddress);
    auto name = p.getModuleNameByAddress(innerAddress);
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
    EXPECT_EQ(p.startThread(threads[0]), true);
    EXPECT_EQ(p.finishThread(threads[0]), true);
    EXPECT_EQ(p.stop(), true);
}

TEST_F(ProfilerTest, GetAllStartedThreads) {
    Profiler p;
    auto threads = createTestThreads(4);
    EXPECT_EQ(p.startThread(threads[0]), false);
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.startThread(threads[1]), true);
    EXPECT_EQ(p.startThread(threads[2]), true);
    EXPECT_EQ(p.stop(), true);
    EXPECT_EQ(p.startThread(threads[3]), false);
    auto startedThreads = p.getStartedThreads();
    EXPECT_EQ(startedThreads.size(), 2);
    EXPECT_EQ(startedThreads[0] == threads[1], true);
    EXPECT_EQ(startedThreads[1] == threads[2], true);
}

}  // namespace
}  // namespace Test
}  // namespace CoMe

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}