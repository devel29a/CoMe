#include "profiler.hpp"

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

    ModulesContainer createTestModules(unsigned amount)
    {
        ModulesContainer container;
        for (unsigned i = 0; i < amount; i++)
            container.emplace_back(Module(0U, 0U, 0U, 0U, std::string("mod") + std::to_string(i)));
        return container;
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
    p.start();
    p.loadModule(modulesContainer[0]);
    p.loadModule(modulesContainer[1]);
    p.stop();
    auto modules = p.getLoadedModules();
    EXPECT_EQ(modules.size(), 2);
    EXPECT_EQ(modules[0] == modulesContainer[0], true);
    EXPECT_EQ(modules[1] == modulesContainer[1], true);
}

TEST_F(ProfilerTest, SomeModulesReportedBeforeAndAfterActiveProfiling) {
    Profiler p;
    const auto modulesContainer = createTestModules(4);
    p.loadModule(modulesContainer[0]);
    p.start();
    p.loadModule(modulesContainer[1]);
    p.loadModule(modulesContainer[2]);
    p.stop();
    p.loadModule(modulesContainer[3]);
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
    p.start();
    p.loadModule(m);
    p.unloadModule(m);
    p.stop();
    EXPECT_EQ(p.getLoadedModules().size(), 0);
}

TEST_F(ProfilerTest, UnloadModuleAllModules) {
    const auto modulesContainer = createTestModules(2);
    Profiler p;
    p.start();
    p.loadModule(modulesContainer[0]);
    p.loadModule(modulesContainer[1]);
    p.unloadAllModules();
    p.stop();
    auto modules = p.getLoadedModules();
    EXPECT_EQ(modules.size(), 0);
}

}  // namespace
}  // namespace Test
}  // namespace CoMe

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}