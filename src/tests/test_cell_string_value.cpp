#include "cell_string_value.hpp"
#include "cell_type.hpp"

namespace garlic {
using CellValuePtr = std::shared_ptr<CellValue>;

#define STR(name) std::string str_##name##_ = #name; std::string_view str_##name = str_##name##_;

STR(hello)
STR(world)
std::string str_empty_ = "";
std::string_view str_empty = str_empty_;

TEST(test_cell_string_value, initialization) {
    CellValuePtr a = std::make_shared<CellStringValue>(str_hello);
    CellValuePtr b = std::make_shared<CellStringValue>(str_world);
    CellValuePtr c = std::make_shared<CellStringValue>(str_empty);
}

TEST(test_cell_string_value, getFunctions_shouldThrowExceptString) {
    CellValuePtr a = std::make_shared<CellStringValue>(str_hello);
    EXPECT_RANGEQ(a->get_string(), std::string_view(str_hello));
    EXPECT_EQ(a->get_type(), CellType::String);
    EXPECT_THROW(a->get_int(), std::logic_error);
    EXPECT_THROW(a->get_float(), std::logic_error);
}

TEST(test_cell_string_value, basicComparingRange5) {
    CellValuePtr a = std::make_shared<CellStringValue>(str_hello);
    CellValuePtr b = std::make_shared<CellStringValue>(str_hello);

    EXPECT_TRUE(a->equals(b));
    EXPECT_TRUE(a->ge(b));
    EXPECT_TRUE(a->le(b));
    EXPECT_FALSE(a->gt(b));
    EXPECT_FALSE(a->lt(b));
}

TEST(test_cell_string_value, basicComparingRangeINTMAX) {
    CellValuePtr amax = std::make_shared<CellStringValue>(str_hello);
    CellValuePtr amin = std::make_shared<CellStringValue>(str_world);

    EXPECT_TRUE(amax->gt(amin));
    EXPECT_TRUE(amax->ge(amin));
    EXPECT_FALSE(amax->lt(amin));
    EXPECT_FALSE(amax->le(amin));
    EXPECT_FALSE(amax->equals(amin));
}

TEST(test_cell_string_value, basicComparingRange10) {
    std::string 
        str_AAA = "AAA",
        str_A = "A";
    CellValuePtr AAA = std::make_shared<CellStringValue>(str_AAA);
    CellValuePtr A = std::make_shared<CellStringValue>(str_A);

    EXPECT_FALSE(AAA->equals(A));
    EXPECT_FALSE(A->equals(AAA));
    EXPECT_TRUE(A->ge(AAA));
    EXPECT_TRUE(A->gt(AAA));
    EXPECT_FALSE(AAA->ge(A));
    EXPECT_FALSE(AAA->gt(A));

    EXPECT_FALSE(A->le(AAA));
    EXPECT_FALSE(A->lt(AAA));
    EXPECT_TRUE(AAA->le(A));
    EXPECT_TRUE(AAA->lt(A));
}

}
