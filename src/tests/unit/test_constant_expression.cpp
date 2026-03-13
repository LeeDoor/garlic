#include "constant_expression.hpp"
#include "table_value_gatherer_mock.hpp"

namespace garlic {

using ExpressionPtr = std::shared_ptr<Expression>;

auto gatherer_mock = std::make_shared<::testing::StrictMock<TableValueGathererMock>>();

TEST(test_constant_expression, IntBasicMethodKit) {
    ExpressionPtr expr_int1 = std::make_shared<IntConstExpr>(5);
    ExpressionPtr expr_int2 = std::make_shared<IntConstExpr>(5);
    ExpressionPtr expr_int3 = std::make_shared<IntConstExpr>(INT_MAX);

    auto value1 = expr_int1->get_value(gatherer_mock);
    auto value2 = expr_int2->get_value(gatherer_mock);
    auto value3 = expr_int3->get_value(gatherer_mock);

    EXPECT_EQ(value1->get_type(), CellType::Int);
    EXPECT_TRUE(value1->equals(value2));
    EXPECT_FALSE(value1->equals(value3));
}

TEST(test_constant_expression, FloatBasicMethodKit) {
    ExpressionPtr expr_float1 = std::make_shared<FloatConstExpr>(M_PI);
    ExpressionPtr expr_float2 = std::make_shared<FloatConstExpr>(M_PI);
    ExpressionPtr expr_float3 = std::make_shared<FloatConstExpr>(M_E);

    auto value1 = expr_float1->get_value(gatherer_mock);
    auto value2 = expr_float2->get_value(gatherer_mock);
    auto value3 = expr_float3->get_value(gatherer_mock);

    EXPECT_EQ(value1->get_type(), CellType::Float);
    EXPECT_TRUE(value1->equals(value2));
    EXPECT_FALSE(value1->equals(value3));
}

TEST(test_constant_expression, StringBasicMethodKit) {
    using namespace std::literals;
    ExpressionPtr expr_string1 = std::make_shared<StringConstExpr>("Hello"s);
    ExpressionPtr expr_string2 = std::make_shared<StringConstExpr>("Hello"s);
    ExpressionPtr expr_string3 = std::make_shared<StringConstExpr>("World"s);

    auto value1 = expr_string1->get_value(gatherer_mock);
    auto value2 = expr_string2->get_value(gatherer_mock);
    auto value3 = expr_string3->get_value(gatherer_mock);

    EXPECT_EQ(value1->get_type(), CellType::String);
    EXPECT_TRUE(value1->equals(value2));
    EXPECT_FALSE(value1->equals(value3));
}

}
