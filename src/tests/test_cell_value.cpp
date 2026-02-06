#include "cell_int_value.hpp"
#include "cell_type.hpp"

namespace garlic {
using CellValuePtr = std::shared_ptr<CellValue>;

TEST(test_cell_int_value, initialization) {
    CellValuePtr a5 = std::make_shared<CellIntValue>(5);
    CellValuePtr b5 = std::make_shared<CellIntValue>(5);
    CellValuePtr amax = std::make_shared<CellIntValue>(std::numeric_limits<IntType>::max());
    CellValuePtr amin = std::make_shared<CellIntValue>(std::numeric_limits<IntType>::min());

    EXPECT_TRUE(a5->compare(b5, BinaryOperation::Equals));
    EXPECT_TRUE(a5->compare(b5, BinaryOperation::Ge));
    EXPECT_TRUE(a5->compare(b5, BinaryOperation::Le));
    EXPECT_FALSE(a5->compare(b5, BinaryOperation::Gt));
    EXPECT_FALSE(a5->compare(b5, BinaryOperation::Ls));

    EXPECT_TRUE(amax->compare(amin, BinaryOperation::Ge));
    EXPECT_TRUE(amax->compare(amin, BinaryOperation::Gt));
    EXPECT_FALSE(amax->compare(amin, BinaryOperation::Ls));
    EXPECT_FALSE(amax->compare(amin, BinaryOperation::Le));
    EXPECT_FALSE(amax->compare(amin, BinaryOperation::Equals));
}

}
