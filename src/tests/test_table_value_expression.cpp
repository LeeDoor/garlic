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
    using GathererPtr = std::shared_ptr<TableValueGathererMock>;

    GathererPtr gatherer_;
    std::shared_ptr<CellIntValue> test_int_value_;
};

TEST_F(TestValueExpressionFixture, init) {
    TableValueExpression expr("Name column");
}

TEST_F(TestValueExpressionFixture, getValue) {
    TableValueExpression expr("Name column");

    EXPECT_CALL(*gatherer_, get_table_value("Name column"))
        .WillOnce(Return(test_int_value_));

    EXPECT_TRUE(expr.get_value(gatherer_)->equals(test_int_value_));
}

TEST_F(TestValueExpressionFixture, thrownExceptionInGatherer_ShouldThrowToo) {
    TableValueExpression expr("Name column");

    EXPECT_CALL(*gatherer_, get_table_value("Name column"))
        .WillOnce(::testing::Throw(std::logic_error("manually generated exception")));

    EXPECT_THROW(expr.get_value(gatherer_), std::logic_error);
}

}
