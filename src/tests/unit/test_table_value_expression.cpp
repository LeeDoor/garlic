#include "cell_int_value.hpp"
#include "table_value_expression.hpp"
#include "table_value_gatherer_mock.hpp"

namespace garlic {

class TestValueExpressionFixture : public ::testing::Test {
public:
    TestValueExpressionFixture()
    : gatherer_{ std::make_shared<TableValueGathererMock>() }
    , test_int_value_{ std::make_shared<CellIntValue>(5) }
    {}

protected:
    sptr<TableValueGathererMock> gatherer_;
    sptr<CellIntValue> test_int_value_;
};

TEST_F(TestValueExpressionFixture, init) {
    TableValueExpression expr("Name column", Int);
}

TEST_F(TestValueExpressionFixture, getValue) {
    TableValueExpression expr("Name column", Int);

    EXPECT_CALL(*gatherer_, get_table_value("Name column"))
        .WillOnce(Return(test_int_value_));

    auto comparable_value = std::dynamic_pointer_cast<CellComparable>(expr.get_value(gatherer_));
    EXPECT_TRUE(comparable_value);
    EXPECT_TRUE(comparable_value->equals(test_int_value_));
}

TEST_F(TestValueExpressionFixture, thrownExceptionInGatherer_ShouldThrowToo) {
    TableValueExpression expr("Name column", Int);

    EXPECT_CALL(*gatherer_, get_table_value("Name column"))
        .WillOnce(::testing::Throw(std::logic_error("manually generated exception")));

    EXPECT_THROW(expr.get_value(gatherer_), std::logic_error);
}

}
