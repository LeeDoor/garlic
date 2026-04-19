#include "compare_condition.hpp"
#include "cell_boolean_value.hpp"
#include "cell_int_value.hpp"
#include "cell_float_value.hpp"
#include "cell_string_view_value.hpp"
#include "constant_expression.hpp"
#include "table_value_gatherer_mock.hpp"

namespace garlic {

static bool unwrap_bool(ExpectedCellValue result) {
    EXPECT_TRUE(result.has_value()) << result.error();
    if(!result) return false;
    auto bool_value = std::dynamic_pointer_cast<CellBooleanValue>(*result);
    EXPECT_NE(bool_value, nullptr);
    return bool_value ? bool_value->get_bool() : false;
}

auto gatherer = std::make_shared<testing::StrictMock<TableValueGathererMock>>();

class TestCompareCondition : public ::testing::Test {
public:
    TestCompareCondition() {}

protected:
    template<typename T>
    uptr<T> unique_cpy(uptr<T>& ptr) {
        return std::make_unique<T>(*ptr);
    }

    template<IsStoringColumnType T> 
    sptr<Condition> create_condition(const T& lhs, const T& rhs, BinaryOperator op) {
        auto lhs_expr = std::make_unique<ConstantExpression<T>>(std::move(lhs));
        auto rhs_expr = std::make_unique<ConstantExpression<T>>(std::move(rhs));
        return std::make_shared<CompareCondition>(std::move(lhs_expr), std::move(rhs_expr), op);
    }
};

TEST_F(TestCompareCondition, init) {
    sptr<Condition> condeq = create_condition(5, 12, Eq);
}

TEST_F(TestCompareCondition, intComparison) {
    sptr<Condition> condeq = create_condition(5, 12, Eq);
    sptr<Condition> condne = create_condition(5, 12, Ne);
    sptr<Condition> condge = create_condition(5, 12, Ge);
    sptr<Condition> condgt = create_condition(5, 12, Gt);
    sptr<Condition> condle = create_condition(5, 12, Le);
    sptr<Condition> condlt = create_condition(5, 12, Lt);

    EXPECT_FALSE(unwrap_bool(condeq->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condne->resolve(gatherer)));
    EXPECT_FALSE(unwrap_bool(condge->resolve(gatherer)));
    EXPECT_FALSE(unwrap_bool(condgt->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condle->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condlt->resolve(gatherer)));
}

TEST_F(TestCompareCondition, floatComparison) {
    sptr<Condition> condeq = create_condition(5.1f, 12.2f, Eq);
    sptr<Condition> condne = create_condition(5.1f, 12.2f, Ne);
    sptr<Condition> condge = create_condition(5.5f, 12.1f, Ge);
    sptr<Condition> condgt = create_condition(5.0f, 12.f, Gt);
    sptr<Condition> condle = create_condition(5.f, 12.f, Le);
    sptr<Condition> condlt = create_condition(5.f, 12.f, Lt);
    auto half_ep = std::numeric_limits<FloatType>::epsilon() / 2;
    sptr<Condition> condsameeq = create_condition(5.f, 5.f + half_ep, Eq);
    sptr<Condition> condsamele = create_condition(5.f, 5.f + half_ep, Le);
    sptr<Condition> condsamege = create_condition(5.f, 5.f + half_ep, Ge);
    sptr<Condition> condsamegt = create_condition(5.f, 5.f + half_ep, Gt);
    sptr<Condition> condsamelt = create_condition(5.f, 5.f + half_ep, Lt);

    EXPECT_FALSE(unwrap_bool(condeq->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condne->resolve(gatherer)));
    EXPECT_FALSE(unwrap_bool(condge->resolve(gatherer)));
    EXPECT_FALSE(unwrap_bool(condgt->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condle->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condlt->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condsameeq->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condsamele->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condsamege->resolve(gatherer)));
    EXPECT_FALSE(unwrap_bool(condsamelt->resolve(gatherer)));
    EXPECT_FALSE(unwrap_bool(condsamegt->resolve(gatherer)));
}

TEST_F(TestCompareCondition, stringComparison) {
    using namespace std::literals;
    sptr<Condition> condeq = create_condition("Hello"s, "Test"s, Eq);
    sptr<Condition> condne = create_condition("Hello"s, "Test"s, Ne);
    sptr<Condition> condle = create_condition("Hello"s, "Test"s, Le);
    sptr<Condition> condge = create_condition("Hello"s, "Test"s, Ge);
    sptr<Condition> condlt = create_condition("Hello"s, "Test"s, Lt);
    sptr<Condition> condgt = create_condition("Hello"s, "Test"s, Gt);

    EXPECT_FALSE(unwrap_bool(condeq->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condne->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condle->resolve(gatherer)));
    EXPECT_FALSE(unwrap_bool(condge->resolve(gatherer)));
    EXPECT_TRUE(unwrap_bool(condlt->resolve(gatherer)));
    EXPECT_FALSE(unwrap_bool(condgt->resolve(gatherer)));
}

}
