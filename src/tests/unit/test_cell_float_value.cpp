#include "cell_float_value.hpp"
#include "cell_type.hpp"

namespace garlic {

TEST(test_cell_float_value, initialization) {
    sptr<CellComparable> a5 = std::make_shared<CellFloatValue>(5.0);
    sptr<CellComparable> b5 = std::make_shared<CellFloatValue>(5.0);
    sptr<CellComparable> amax = std::make_shared<CellFloatValue>(1e10);
    sptr<CellComparable> amin = std::make_shared<CellFloatValue>(-1e10);
}

TEST(test_cell_float_value, basicComparingRange5) {
    sptr<CellComparable> a5 = std::make_shared<CellFloatValue>(5.0);
    sptr<CellComparable> b5 = std::make_shared<CellFloatValue>(5.0);

    EXPECT_TRUE(a5->equals(b5));
    EXPECT_TRUE(a5->ge(b5));
    EXPECT_TRUE(a5->le(b5));
    EXPECT_FALSE(a5->gt(b5));
    EXPECT_FALSE(a5->lt(b5));
}

TEST(test_cell_float_value, basicComparingWithMiserableDifference_ShouldAccountAsSame) {
    sptr<CellComparable> a5 = std::make_shared<CellFloatValue>(5.0 - 1e-17);
    sptr<CellComparable> b5 = std::make_shared<CellFloatValue>(5.0);

    EXPECT_TRUE(a5->equals(b5));
    EXPECT_TRUE(a5->ge(b5));
    EXPECT_TRUE(a5->le(b5));
    EXPECT_FALSE(a5->gt(b5));
    EXPECT_FALSE(a5->lt(b5));
}

TEST(test_cell_float_value, basicComparingRangeINTMAX) {
    sptr<CellComparable> amax = std::make_shared<CellFloatValue>(std::numeric_limits<FloatType>::max());
    sptr<CellComparable> amin = std::make_shared<CellFloatValue>(std::numeric_limits<FloatType>::min());

    EXPECT_TRUE(amax->ge(amin));
    EXPECT_TRUE(amax->gt(amin));
    EXPECT_FALSE(amax->lt(amin));
    EXPECT_FALSE(amax->le(amin));
    EXPECT_FALSE(amax->equals(amin));
}

TEST(test_cell_float_value, basicComparingRange10) {
    sptr<CellComparable> a4 = std::make_shared<CellFloatValue>(4.5);
    sptr<CellComparable> a0 = std::make_shared<CellFloatValue>(0.1);
    sptr<CellComparable> a5 = std::make_shared<CellFloatValue>(5.2);
    sptr<CellComparable> an7 = std::make_shared<CellFloatValue>(-7.5);

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
