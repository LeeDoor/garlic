#include "constant_expression.hpp"
#include "mock_condition_resolver.hpp"

namespace garlic {

using ExpressionPtr = std::shared_ptr<Expression>;

auto resolver = std::make_shared<testing::StrictMock<MockConditionResolver>>();

TEST(test_constant_expression, IntBasicMethodKit) {
    ExpressionPtr expr_int1 = std::make_shared<IntConstExpr>(resolver, 5);
    ExpressionPtr expr_int2 = std::make_shared<IntConstExpr>(resolver, 5);
    ExpressionPtr expr_int3 = std::make_shared<IntConstExpr>(resolver, INT_MAX);

    auto value1 = expr_int1->get_value();
    auto value2 = expr_int2->get_value();
    auto value3 = expr_int3->get_value();

    EXPECT_EQ(expr_int1->get_type(), CellType::Int);
    EXPECT_EQ(value1->get_type(), CellType::Int);
    EXPECT_TRUE(value1->equals(value2));
    EXPECT_FALSE(value1->equals(value3));
}

TEST(test_constant_expression, FloatBasicMethodKit) {
    ExpressionPtr expr_float1 = std::make_shared<FloatConstExpr>(resolver, M_PI);
    ExpressionPtr expr_float2 = std::make_shared<FloatConstExpr>(resolver, M_PI);
    ExpressionPtr expr_float3 = std::make_shared<FloatConstExpr>(resolver, M_E);

    auto value1 = expr_float1->get_value();
    auto value2 = expr_float2->get_value();
    auto value3 = expr_float3->get_value();

    EXPECT_EQ(expr_float1->get_type(), CellType::Float);
    EXPECT_EQ(value1->get_type(), CellType::Float);
    EXPECT_TRUE(value1->equals(value2));
    EXPECT_FALSE(value1->equals(value3));
}

TEST(test_constant_expression, StringBasicMethodKit) {
    std::string str_hello = "Hello", str_world = "World";
    ExpressionPtr expr_string1 = std::make_shared<StringConstExpr>(resolver, str_hello);
    ExpressionPtr expr_string2 = std::make_shared<StringConstExpr>(resolver, str_hello);
    ExpressionPtr expr_string3 = std::make_shared<StringConstExpr>(resolver, str_world);

    auto value1 = expr_string1->get_value();
    auto value2 = expr_string2->get_value();
    auto value3 = expr_string3->get_value();

    EXPECT_EQ(expr_string1->get_type(), CellType::String);
    EXPECT_EQ(value1->get_type(), CellType::String);
    EXPECT_TRUE(value1->equals(value2));
    EXPECT_FALSE(value1->equals(value3));
}

}
