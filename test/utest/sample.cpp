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

#include "sample.hpp"

#include <gtest/gtest.h>

namespace CoMe {
namespace Test {
namespace {

class SampleTest : public testing::Test {
protected:
    SampleTest() {}
    ~SampleTest() override {}
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SampleTest, CreateSampleObject) {
    Sample s { 10U, 20U, 30U };
    EXPECT_EQ(s.TSC, 10U);
    EXPECT_EQ(s.SP, 20U);
    EXPECT_EQ(s.BP, 30U);
}

TEST_F(SampleTest, CopySampleObject) {
    Sample s1 { 10U, 20U, 30U };
    Sample s2 = s1;
    EXPECT_EQ(s2.TSC, 10U);
    EXPECT_EQ(s2.SP, 20U);
    EXPECT_EQ(s2.BP, 30U);
}

TEST_F(SampleTest, CompareEqualSampleObject) {
    Sample s1 { 10U, 20U, 30U };
    Sample s2 = s1;
    EXPECT_EQ(s1 == s2, true);
}

TEST_F(SampleTest, CompareLessThanSampleObject) {
    Sample s1 { 10U, 20U, 30U };
    Sample s2 { 15U, 20U, 30U };
    EXPECT_EQ(s1 < s2, true);
}

}  // namespace
}  // namespace Test
}  // namespace CoMe

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}