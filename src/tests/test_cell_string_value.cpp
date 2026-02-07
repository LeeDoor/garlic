#include "cell_string_value.hpp"
#include "cell_type.hpp"

namespace garlic {
using CellValuePtr = std::shared_ptr<CellValue>;

#define STR(name) std::string str_##name##_ = #name; std::string_view str_##name = str_##name##_;

STR(hello)
STR(world)
StringType str_empty_ = "";
StringViewType str_empty = str_empty_;

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
    std::string 
        str_AAA = "AAA",
        str_A = "A";
    CellValuePtr AAA = std::make_shared<CellStringValue>(str_AAA);
    CellValuePtr A = std::make_shared<CellStringValue>(str_A);
    
    std::cerr << std::strcmp(str_AAA.data(), str_A.data()) << std::endl;

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
