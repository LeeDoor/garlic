#include "cell_int_value.hpp"
#include "table_value_expression.hpp"
#include "cell_value_gatherer_mock.hpp"
#include "tables_gatherer_mock.hpp"

namespace garlic {

class TestValueExpressionFixture : public ::testing::Test {
public:
    TestValueExpressionFixture()
    : gatherer_{ std::make_shared<CellValueGathererMock>() }
    , test_int_value_{ std::make_shared<CellIntValue>(5) }
    {}

protected:
    sptr<CellValueGathererMock> gatherer_;
    sptr<CellIntValue> test_int_value_;
};

TEST_F(TestValueExpressionFixture, init) {
    TableValueExpression expr(TablesGathererMock{ Int }, "Table name", "Name column");
}

TEST_F(TestValueExpressionFixture, getValue) {
    TableValueExpression expr(TablesGathererMock{ Int }, "Table name", "Name column");

    EXPECT_CALL(*gatherer_, get_table_value("Name column"))
        .WillOnce(Return(test_int_value_));

    auto value = expr.resolve(gatherer_);
    ASSERT_TRUE(value.has_value()) << value.error();
    auto comparable_value = std::dynamic_pointer_cast<CellComparable>(*value);
    EXPECT_TRUE(comparable_value);
    EXPECT_TRUE(comparable_value->equals(test_int_value_));
}

TEST_F(TestValueExpressionFixture, thrownExceptionInGatherer_ShouldThrowToo) {
    TableValueExpression expr(TablesGathererMock{ Int }, "Table name", "Name column");

    EXPECT_CALL(*gatherer_, get_table_value("Name column"))
        .WillOnce(::testing::Throw(std::logic_error("manually generated exception")));

    EXPECT_THROW(expr.resolve(gatherer_), std::logic_error);
}

}
