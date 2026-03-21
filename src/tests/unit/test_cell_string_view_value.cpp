#include "cell_string_view_value.hpp"
#include "cell_type.hpp"

namespace garlic {
using namespace std::literals;

auto str_hello = "Hello"sv;
auto str_world = "World"sv;

TEST(test_cell_string_view_value, initialization) {
    sptr<CellValue> a = std::make_shared<CellStringViewValue>(str_hello);
    sptr<CellValue> b = std::make_shared<CellStringViewValue>(str_world);
    sptr<CellValue> c = std::make_shared<CellStringViewValue>(""sv);
}

TEST(test_cell_string_view_value, getType_shouldGetString) {
    sptr<CellValue> a = std::make_shared<CellStringViewValue>(str_hello);
    EXPECT_EQ(a->get_type(), CellType::String);
}

TEST(test_cell_string_view_value, comparingSameStrings) {
    sptr<CellValue> a = std::make_shared<CellStringViewValue>(str_hello);
    sptr<CellValue> b = std::make_shared<CellStringViewValue>(str_hello);

    EXPECT_TRUE(a->equals(b));
    EXPECT_TRUE(a->ge(b));
    EXPECT_TRUE(a->le(b));
    EXPECT_FALSE(a->gt(b));
    EXPECT_FALSE(a->lt(b));
}

TEST(test_cell_string_view_value, basicComparingHelloAndWord) {
    sptr<CellValue> amin = std::make_shared<CellStringViewValue>(str_hello);
    sptr<CellValue> amax = std::make_shared<CellStringViewValue>(str_world);

    EXPECT_TRUE(amax->gt(amin));
    EXPECT_TRUE(amax->ge(amin));
    EXPECT_FALSE(amax->lt(amin));
    EXPECT_FALSE(amax->le(amin));
    EXPECT_FALSE(amax->equals(amin));
}

TEST(test_cell_string_view_value, SameLettersDifferentSize) {
    sptr<CellValue> AAA = std::make_shared<CellStringViewValue>("AAA"sv);
    sptr<CellValue> A = std::make_shared<CellStringViewValue>("A"sv);
    
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
