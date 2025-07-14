#include "module.hpp"
#include "profiler.hpp"

#include <gtest/gtest.h>

namespace CoMe {
namespace Test {
namespace {

class ModulesTest : public testing::Test {
 protected:
  ModulesTest() {}
  ~ModulesTest() override {}
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(ModulesTest, CreateModuleObject) {
    Module m {12U, 34U, 56U, 78U, std::string("mod1")};
    EXPECT_EQ(m.StartAddress, 12U);
    EXPECT_EQ(m.EndAddress,   34U);
    EXPECT_EQ(m.LoadTSC,      56U);
    EXPECT_EQ(m.UnloadTSC,    78U);
    EXPECT_EQ(m.FullPath,  std::string("mod1"));
}

TEST_F(ModulesTest, CopyModuleObject) {
    Module m1 {12U, 34U, 56U, 78U, std::string("mod1")};
    Module m2 { m1 };
    EXPECT_EQ(m1==m2, true);
}

TEST_F(ModulesTest, GetEmptyModulesList) {
    Profiler p;
    p.start();
    p.stop();
    EXPECT_EQ(p.getModules().size(), 0);

}

TEST_F(ModulesTest, AllModulesReportedDuringActiveProfiling) {
    const Profiler::ModulesContainer modulesContainer {
        { 0U, 0U, 0U, 0U, std::string("mod0") },
        { 0U, 0U, 0U, 0U, std::string("mod1") }
    };
    Profiler p;
    p.start();
    p.loadModule(modulesContainer[0]);
    p.loadModule(modulesContainer[1]);
    p.stop();
    auto modules = p.getModules();
    EXPECT_EQ(modules.size(), 2);
    EXPECT_EQ(modules[0] == modulesContainer[0], true);
    EXPECT_EQ(modules[1] == modulesContainer[1], true);
}

TEST_F(ModulesTest, SomeModulesReportedBeforeAndAfterActiveProfiling) {
    Profiler p;
    const Profiler::ModulesContainer modulesContainer {
            { 0U, 0U, 0U, 0U, std::string("mod0") },
            { 0U, 0U, 0U, 0U, std::string("mod1") },
            { 0U, 0U, 0U, 0U, std::string("mod2") },
            { 0U, 0U, 0U, 0U, std::string("mod3") }
        };
    p.loadModule(modulesContainer[0]);
    p.start();
    p.loadModule(modulesContainer[1]);
    p.loadModule(modulesContainer[2]);
    p.stop();
    p.loadModule(modulesContainer[3]);
    auto modules = p.getModules();
    EXPECT_EQ(modules.size(), 2);
    EXPECT_EQ(modules[0] == modulesContainer[1], true);
    EXPECT_EQ(modules[1] == modulesContainer[2], true);
}

}  // namespace
}  // namespace Test
}  // namespace CoMe

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}