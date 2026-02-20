#include "compare_condition.hpp"
#include "cell_int_value.hpp"
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

    template<IsColumnType T> 
    ConditionPtr create_condition(T lhs, T rhs, BinaryOperation op) {
        std::unique_ptr<typename get_cell_type<T>::Type> lexpr = std::make_unique<typename get_cell_type<T>::Type>(lhs);
        std::unique_ptr<typename get_cell_type<T>::Type> rexpr = std::make_unique<typename get_cell_type<T>::Type>(rhs);
        return std::make_shared<CompareCondition>(std::move(lexpr), std::move(rexpr), op);
    }
};

TEST_F(TestCompareCondition, init) {
    ConditionPtr condeq = create_condition(5, 12, Equals);
}

TEST_F(TestCompareCondition, intComparison) {
    ConditionPtr condeq = create_condition<IntType>(5, 12, Equals);
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

}
