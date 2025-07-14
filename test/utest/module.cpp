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

}  // namespace
}  // namespace Test
}  // namespace CoMe

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}