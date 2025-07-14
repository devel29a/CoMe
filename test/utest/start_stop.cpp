#include "profiler.hpp"
#include <gtest/gtest.h>

namespace CoMe {
namespace Test {
namespace {

class StartStopTest : public testing::Test {
 protected:
  StartStopTest() {}
  ~StartStopTest() override {}
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(StartStopTest, StartStopProfiling) {
    Profiler p;
    EXPECT_EQ(p.start(), true);
    EXPECT_EQ(p.stop(), true);

}
}  // namespace
}  // namespace project
}  // namespace my

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}