#include "symbol.hpp"

#include <gtest/gtest.h>

namespace CoMe {
namespace Test {
namespace {

class SymbolTest : public testing::Test {
protected:
    SymbolTest() {}
    ~SymbolTest() override {}
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SymbolTest, CreateSymbolObject) {
    Symbol s { std::string("sym0"), 0U, std::string("mod0") };
    EXPECT_EQ(s.Name, std::string("sym0"));
    EXPECT_EQ(s.Address, 0U);
    EXPECT_EQ(s.Module, std::string("mod0"));
}

TEST_F(SymbolTest, CopySymbolObject) {
    Symbol sym1 { std::string("sym0"), 0U, std::string("mod0") };
    Symbol sym2 = sym1;
    EXPECT_EQ(sym2.Name, std::string("sym0"));
    EXPECT_EQ(sym2.Address, 0U);
    EXPECT_EQ(sym2.Module, std::string("mod0"));
}

TEST_F(SymbolTest, CompareSymbolObject) {
    Symbol sym1 { std::string("sym0"), 0U, std::string("mod0") };
    Symbol sym2 = sym1;
    EXPECT_EQ(sym1 == sym2, true);
}

}  // namespace
}  // namespace Test
}  // namespace CoMe

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}