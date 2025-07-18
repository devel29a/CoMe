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

#include "thread.hpp"

#include <gtest/gtest.h>

namespace CoMe {
namespace Test {
namespace {

class ThreadTest : public testing::Test {
protected:
    ThreadTest() {}
    ~ThreadTest() override {}
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ThreadTest, CreateThreadObject) {
    Thread t { 12U, 34U, 56U };

    EXPECT_EQ(t.StartTSC, 12U);
    EXPECT_EQ(t.FinishTSC, 34U);
    EXPECT_EQ(t.Context, 56U);
}

TEST_F(ThreadTest, CopyThreadObject) {
    Thread t1 { 12U, 34U, 56U };
    Thread t2 = t1;

    EXPECT_EQ(t2.StartTSC, 12U);
    EXPECT_EQ(t2.FinishTSC, 34U);
    EXPECT_EQ(t2.Context, 56U);
}

TEST_F(ThreadTest, CompareThreadObject) {
    Thread t1 { 12U, 34U, 56U };
    Thread t2 = t1;

    EXPECT_EQ(t1 == t2, true);
}

}  // namespace
}  // namespace Test
}  // namespace CoMe

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}