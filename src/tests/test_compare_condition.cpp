#include "compare_condition.hpp"
#include "cell_int_value.hpp"
#include "cell_float_value.hpp"
#include "cell_string_value.hpp"
#include "table_value_gatherer_mock.hpp"

namespace garlic {

using ConditionPtr = std::shared_ptr<Condition>;
using ExpressionPtr = std::unique_ptr<Expression>;

auto gatherer = std::make_shared<testing::StrictMock<TableValueGathererMock>>();

class TestCompareCondition : public ::testing::Test {
public:
    TestCompareCondition() {

    }

protected:
    template<typename T>
    std::unique_ptr<T> deep_copy(std::unique_ptr<T>& ptr) {
        return std::make_unique<T>(*ptr);
    }

    template<IsReadonlyColumnType T> 
    ConditionPtr create_condition(const T& lhs, const T& rhs, BinaryOperation op) {
        std::unique_ptr<typename get_cell_type<T>::Type> lexpr = std::make_unique<typename get_cell_type<T>::Type>(lhs);
        std::unique_ptr<typename get_cell_type<T>::Type> rexpr = std::make_unique<typename get_cell_type<T>::Type>(rhs);
        return std::make_shared<CompareCondition>(std::move(lexpr), std::move(rexpr), op);
    }
#define STR(name) StringType str_##name##_ = #name; StringViewType str_##name = str_##name##_;
    STR(test)
    STR(hello)
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
    ConditionPtr condsameeq = create_condition(5.f, 5.f + std::numeric_limits<FloatType>::epsilon() / 2, Equals);
    ConditionPtr condsamele = create_condition(5.f, 5.f + std::numeric_limits<FloatType>::epsilon() / 2, Le);
    ConditionPtr condsamege = create_condition(5.f, 5.f + std::numeric_limits<FloatType>::epsilon() / 2, Ge);
    ConditionPtr condsamegt = create_condition(5.f, 5.f + std::numeric_limits<FloatType>::epsilon() / 2, Gt);
    ConditionPtr condsamelt = create_condition(5.f, 5.f + std::numeric_limits<FloatType>::epsilon() / 2, Lt);

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
    ConditionPtr condeq = create_condition<StringViewType>(str_hello, str_test, Equals);
    ConditionPtr condle = create_condition<StringViewType>(str_hello, str_test, Le);
    ConditionPtr condge = create_condition<StringViewType>(str_hello, str_test, Ge);
    ConditionPtr condlt = create_condition<StringViewType>(str_hello, str_test, Lt);
    ConditionPtr condgt = create_condition<StringViewType>(str_hello, str_test, Le);

    EXPECT_FALSE(condeq->resolve(gatherer));
    EXPECT_TRUE(condle->resolve(gatherer));
    EXPECT_FALSE(condge->resolve(gatherer));
    EXPECT_TRUE(condlt->resolve(gatherer));
    EXPECT_FALSE(condgt->resolve(gatherer));
}

}
