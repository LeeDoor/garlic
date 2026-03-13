#include "compare_condition.hpp"
#include "cell_int_value.hpp"
#include "cell_float_value.hpp"
#include "cell_string_value.hpp"
#include "constant_expression.hpp"
#include "table_value_gatherer_mock.hpp"

namespace garlic {

using ConditionPtr = std::shared_ptr<Condition>;
using ExpressionPtr = std::unique_ptr<Expression>;

auto gatherer = std::make_shared<testing::StrictMock<TableValueGathererMock>>();

class TestCompareCondition : public ::testing::Test {
public:
    TestCompareCondition() {}

protected:
    template<typename T>
    std::unique_ptr<T> unique_cpy(std::unique_ptr<T>& ptr) {
        return std::make_unique<T>(*ptr);
    }

    template<IsStoringColumnType T> 
    ConditionPtr create_condition(const T& lhs, const T& rhs, BinaryOperator op) {
        auto lhs_expr = std::make_unique<ConstantExpression<T>>(std::move(lhs));
        auto rhs_expr = std::make_unique<ConstantExpression<T>>(std::move(rhs));
        return std::make_shared<CompareCondition>(std::move(lhs_expr), std::move(rhs_expr), op);
    }
};

TEST_F(TestCompareCondition, init) {
    ConditionPtr condeq = create_condition(5, 12, Equals);
}

TEST_F(TestCompareCondition, intComparison) {
    ConditionPtr condeq = create_condition(5, 12, Equals);
    ConditionPtr condge = create_condition(5, 12, Ge);
    ConditionPtr condgt = create_condition(5, 12, Gt);
    ConditionPtr condle = create_condition(5, 12, Le);
    ConditionPtr condlt = create_condition(5, 12, Lt);

    EXPECT_FALSE(condeq->resolve(gatherer));
    EXPECT_FALSE(condge->resolve(gatherer));
    EXPECT_FALSE(condgt->resolve(gatherer));
    EXPECT_TRUE(condle->resolve(gatherer));
    EXPECT_TRUE(condlt->resolve(gatherer));
}

TEST_F(TestCompareCondition, floatComparison) {
    ConditionPtr condeq = create_condition(5.1f, 12.2f, Equals);
    ConditionPtr condge = create_condition(5.5f, 12.1f, Ge);
    ConditionPtr condgt = create_condition(5.0f, 12.f, Gt);
    ConditionPtr condle = create_condition(5.f, 12.f, Le);
    ConditionPtr condlt = create_condition(5.f, 12.f, Lt);
    auto half_ep = std::numeric_limits<FloatType>::epsilon() / 2;
    ConditionPtr condsameeq = create_condition(5.f, 5.f + half_ep, Equals);
    ConditionPtr condsamele = create_condition(5.f, 5.f + half_ep, Le);
    ConditionPtr condsamege = create_condition(5.f, 5.f + half_ep, Ge);
    ConditionPtr condsamegt = create_condition(5.f, 5.f + half_ep, Gt);
    ConditionPtr condsamelt = create_condition(5.f, 5.f + half_ep, Lt);

    EXPECT_FALSE(condeq->resolve(gatherer));
    EXPECT_FALSE(condge->resolve(gatherer));
    EXPECT_FALSE(condgt->resolve(gatherer));
    EXPECT_TRUE(condle->resolve(gatherer));
    EXPECT_TRUE(condlt->resolve(gatherer));
    EXPECT_TRUE(condsameeq->resolve(gatherer));
    EXPECT_TRUE(condsamele->resolve(gatherer));
    EXPECT_TRUE(condsamege->resolve(gatherer));
    EXPECT_FALSE(condsamelt->resolve(gatherer));
    EXPECT_FALSE(condsamegt->resolve(gatherer));
}

TEST_F(TestCompareCondition, stringComparison) {
    using namespace std::literals;
    ConditionPtr condeq = create_condition("Hello"s, "Test"s, Equals);
    ConditionPtr condle = create_condition("Hello"s, "Test"s, Le);
    ConditionPtr condge = create_condition("Hello"s, "Test"s, Ge);
    ConditionPtr condlt = create_condition("Hello"s, "Test"s, Lt);
    ConditionPtr condgt = create_condition("Hello"s, "Test"s, Gt);

    EXPECT_FALSE(condeq->resolve(gatherer));
    EXPECT_TRUE(condle->resolve(gatherer));
    EXPECT_FALSE(condge->resolve(gatherer));
    EXPECT_TRUE(condlt->resolve(gatherer));
    EXPECT_FALSE(condgt->resolve(gatherer));
}

}
