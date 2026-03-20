#include "cell_int_value.hpp"
#include "cell_type.hpp"

namespace garlic {

TEST(test_cell_int_value, initialization) {
    sptr<CellValue> a5 = std::make_shared<CellIntValue>(5);
    sptr<CellValue> b5 = std::make_shared<CellIntValue>(5);
    sptr<CellValue> amax = std::make_shared<CellIntValue>(std::numeric_limits<IntType>::max());
    sptr<CellValue> amin = std::make_shared<CellIntValue>(std::numeric_limits<IntType>::min());
}

TEST(test_cell_int_value, getType_ShouldBeInt) {
    sptr<CellValue> a5 = std::make_shared<CellIntValue>(5);
    EXPECT_EQ(a5->get_type(), CellType::Int);
}

TEST(test_cell_int_value, basicComparingRange5) {
    sptr<CellValue> a5 = std::make_shared<CellIntValue>(5);
    sptr<CellValue> b5 = std::make_shared<CellIntValue>(5);

    EXPECT_TRUE(a5->equals(b5));
    EXPECT_TRUE(a5->ge(b5));
    EXPECT_TRUE(a5->le(b5));
    EXPECT_FALSE(a5->gt(b5));
    EXPECT_FALSE(a5->lt(b5));
}

TEST(test_cell_int_value, basicComparingRangeINTMAX) {
    sptr<CellValue> amax = std::make_shared<CellIntValue>(std::numeric_limits<IntType>::max());
    sptr<CellValue> amin = std::make_shared<CellIntValue>(std::numeric_limits<IntType>::min());

    EXPECT_TRUE(amax->ge(amin));
    EXPECT_TRUE(amax->gt(amin));
    EXPECT_FALSE(amax->lt(amin));
    EXPECT_FALSE(amax->le(amin));
    EXPECT_FALSE(amax->equals(amin));
}

TEST(test_cell_int_value, basicComparingRange10) {
    sptr<CellValue> a4 = std::make_shared<CellIntValue>(4);
    sptr<CellValue> a0 = std::make_shared<CellIntValue>(0);
    sptr<CellValue> a5 = std::make_shared<CellIntValue>(5);
    sptr<CellValue> an7 = std::make_shared<CellIntValue>(-7);

    EXPECT_FALSE(a4->equals(a0));
    EXPECT_TRUE(a4->ge(a0));
    EXPECT_TRUE(a4->gt(a0));
    EXPECT_FALSE(a4->le(a0));
    EXPECT_FALSE(a4->lt(a0));

    EXPECT_FALSE(a4->equals(a5));
    EXPECT_TRUE(a4->le(a5));
    EXPECT_TRUE(a4->lt(a5));
    EXPECT_FALSE(a4->ge(a5));
    EXPECT_FALSE(a4->gt(a5));

    EXPECT_FALSE(a4->equals(an7));
    EXPECT_FALSE(a4->le(an7));
    EXPECT_FALSE(a4->lt(an7));
    EXPECT_TRUE(a4->ge(an7));
    EXPECT_TRUE(a4->gt(an7));
}

}
