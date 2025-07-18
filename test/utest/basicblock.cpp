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