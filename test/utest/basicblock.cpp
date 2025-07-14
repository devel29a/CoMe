#include "basicblock.hpp"

#include <gtest/gtest.h>

namespace CoMe {
namespace Test {
namespace {

class BasicBlocksTest : public testing::Test {
protected:
    BasicBlocksTest() {}
    ~BasicBlocksTest() override {}
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(BasicBlocksTest, CreateBasicBlockObject) {
    BasicBlock bb { 0U, 10U};
}

TEST_F(BasicBlocksTest, CopyBasicBlockObject) {
    BasicBlock bb1 { 0U, 10U};
    BasicBlock bb2 = bb1;
    EXPECT_EQ(bb2.StartAddress, 0U);
    EXPECT_EQ(bb2.EndAddress, 10U);
}

TEST_F(BasicBlocksTest, CompareBasicBlockObject) {
    BasicBlock bb1 { 0U, 10U};
    BasicBlock bb2 = bb1;
    EXPECT_EQ(bb1 == bb2, true);
}

}  // namespace
}  // namespace Test
}  // namespace CoMe

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}