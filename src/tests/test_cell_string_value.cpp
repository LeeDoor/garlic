#include "cell_string_value.hpp"
#include "cell_type.hpp"

namespace garlic {
using namespace std::literals;
using CellValuePtr = std::shared_ptr<CellValue>;

auto str_hello = "Hello"sv;
auto str_world = "World"sv;

TEST(test_cell_string_value, initialization) {
    CellValuePtr a = std::make_shared<CellStringValue>(str_hello);
    CellValuePtr b = std::make_shared<CellStringValue>(str_world);
    CellValuePtr c = std::make_shared<CellStringValue>(""sv);
}

TEST(test_cell_string_value, getType_shouldGetString) {
    CellValuePtr a = std::make_shared<CellStringValue>(str_hello);
    EXPECT_EQ(a->get_type(), CellType::String);
}

TEST(test_cell_string_value, comparingSameStrings) {
    CellValuePtr a = std::make_shared<CellStringValue>(str_hello);
    CellValuePtr b = std::make_shared<CellStringValue>(str_hello);

    EXPECT_TRUE(a->equals(b));
    EXPECT_TRUE(a->ge(b));
    EXPECT_TRUE(a->le(b));
    EXPECT_FALSE(a->gt(b));
    EXPECT_FALSE(a->lt(b));
}

TEST(test_cell_string_value, basicComparingHelloAndWord) {
    CellValuePtr amin = std::make_shared<CellStringValue>(str_hello);
    CellValuePtr amax = std::make_shared<CellStringValue>(str_world);

    EXPECT_TRUE(amax->gt(amin));
    EXPECT_TRUE(amax->ge(amin));
    EXPECT_FALSE(amax->lt(amin));
    EXPECT_FALSE(amax->le(amin));
    EXPECT_FALSE(amax->equals(amin));
}

TEST(test_cell_string_value, SameLettersDifferentSize) {
    CellValuePtr AAA = std::make_shared<CellStringValue>("AAA"sv);
    CellValuePtr A = std::make_shared<CellStringValue>("A"sv);
    
    EXPECT_FALSE(AAA->equals(A));
    EXPECT_FALSE(A->equals(AAA));
    EXPECT_TRUE(A->le(AAA));
    EXPECT_TRUE(A->lt(AAA));
    EXPECT_FALSE(AAA->le(A));
    EXPECT_FALSE(AAA->lt(A));

    EXPECT_FALSE(A->ge(AAA));
    EXPECT_FALSE(A->gt(AAA));
    EXPECT_TRUE(AAA->ge(A));
    EXPECT_TRUE(AAA->gt(A));
}

}
