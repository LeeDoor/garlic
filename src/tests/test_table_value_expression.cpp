#include "cell_int_value.hpp"
#include "table_value_expression.hpp"
#include "table_value_gatherer_mock.hpp"

namespace garlic {

class TestValueExpressionFixture : public ::testing::Test {
public:
    TestValueExpressionFixture()
    : gatherer_{ std::make_shared<TableValueGathererMock>() }
    {}

protected:
    using GathererPtr = std::shared_ptr<TableValueGathererMock>;

    GathererPtr gatherer_;
};

TEST_F(TestValueExpressionFixture, init) {
    TableValueExpression expr("Name column");
}

TEST_F(TestValueExpressionFixture, getValue) {
    TableValueExpression expr("Name column");

    auto return_value = std::make_shared<CellIntValue>(5);
    EXPECT_CALL(*gatherer_, get_table_value("Name column"))
        .WillOnce(Return(return_value));

    EXPECT_TRUE(expr.get_value(gatherer_)->equals(return_value));
}

}
