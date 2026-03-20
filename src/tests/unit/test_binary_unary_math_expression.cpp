#include "binary_math_expression.hpp"
#include "unary_math_expression.hpp"
#include "constant_expression.hpp"
#include "table_value_gatherer_mock.hpp"

namespace garlic {

static IntType as_int(const sptr<CellValue>& value) {
    auto p = std::dynamic_pointer_cast<CellIntValue>(value);
    EXPECT_NE(p, nullptr);
    return p->get_int();
}

static FloatType as_float(const sptr<CellValue>& value) {
    auto p = std::dynamic_pointer_cast<CellFloatValue>(value);
    EXPECT_NE(p, nullptr);
    return p->get_float();
}

TEST(test_binary_math_expression, intOperators) {
    auto g = std::make_shared<testing::StrictMock<TableValueGathererMock>>();
    auto lhs = std::make_shared<IntConstExpr>(17);
    auto rhs = std::make_shared<IntConstExpr>(5);

    EXPECT_EQ(as_int(BinaryMathExpression(lhs, rhs, ADD).get_value(g)), 22);
    EXPECT_EQ(as_int(BinaryMathExpression(lhs, rhs, SUB).get_value(g)), 12);
    EXPECT_EQ(as_int(BinaryMathExpression(lhs, rhs, MUL).get_value(g)), 85);
    EXPECT_EQ(as_int(BinaryMathExpression(lhs, rhs, DIV).get_value(g)), 3);
    EXPECT_EQ(as_int(BinaryMathExpression(lhs, rhs, REMDIV).get_value(g)), 2);
}

TEST(test_binary_math_expression, mixedIntFloatReturnsFloat) {
    auto g = std::make_shared<testing::StrictMock<TableValueGathererMock>>();
    auto lhs = std::make_shared<IntConstExpr>(5);
    auto rhs = std::make_shared<FloatConstExpr>(2.0f);

    auto add_res = BinaryMathExpression(lhs, rhs, ADD).get_value(g);
    auto div_res = BinaryMathExpression(lhs, rhs, DIV).get_value(g);
    auto rem_res = BinaryMathExpression(lhs, rhs, REMDIV).get_value(g);

    EXPECT_EQ(add_res->get_type(), Float);
    EXPECT_EQ(div_res->get_type(), Float);
    EXPECT_EQ(rem_res->get_type(), Float);
    EXPECT_FLOAT_EQ(as_float(add_res), 7.0f);
    EXPECT_FLOAT_EQ(as_float(div_res), 2.5f);
    EXPECT_FLOAT_EQ(as_float(rem_res), 1.0f);
}

TEST(test_binary_math_expression, operandWithoutMathSupportShouldThrow) {
    auto g = std::make_shared<testing::StrictMock<TableValueGathererMock>>();
    auto lhs = std::make_shared<StringConstExpr>("abc");
    auto rhs = std::make_shared<IntConstExpr>(2);

    BinaryMathExpression expr(lhs, rhs, ADD);
    EXPECT_THROW(expr.get_value(g), std::logic_error);
}

TEST(test_unary_math_expression, absAndNegForInt) {
    auto g = std::make_shared<testing::StrictMock<TableValueGathererMock>>();
    auto val = std::make_shared<IntConstExpr>(-17);

    EXPECT_EQ(as_int(UnaryMathExpression(val, ABS).get_value(g)), 17);
    EXPECT_EQ(as_int(UnaryMathExpression(val, NEG).get_value(g)), 17);
}

TEST(test_unary_math_expression, absAndNegForFloat) {
    auto g = std::make_shared<testing::StrictMock<TableValueGathererMock>>();
    auto val = std::make_shared<FloatConstExpr>(-2.5f);

    EXPECT_FLOAT_EQ(as_float(UnaryMathExpression(val, ABS).get_value(g)), 2.5f);
    EXPECT_FLOAT_EQ(as_float(UnaryMathExpression(val, NEG).get_value(g)), 2.5f);
}

TEST(test_unary_math_expression, operandWithoutMathSupportShouldThrow) {
    auto g = std::make_shared<testing::StrictMock<TableValueGathererMock>>();
    auto val = std::make_shared<StringConstExpr>("abc");

    UnaryMathExpression expr(val, NEG);
    EXPECT_THROW(expr.get_value(g), std::logic_error);
}

TEST(test_binary_unary_math_expression, composedBigExpression) {
    auto g = std::make_shared<testing::StrictMock<TableValueGathererMock>>();

    // ((|-(1000)| + 24) * 3) % 7 = 0
    auto base = std::make_shared<IntConstExpr>(1000);
    auto neg = std::make_shared<UnaryMathExpression>(base, NEG);
    auto abs = std::make_shared<UnaryMathExpression>(neg, ABS);
    auto plus = std::make_shared<BinaryMathExpression>(abs, std::make_shared<IntConstExpr>(24), ADD);
    auto mul = std::make_shared<BinaryMathExpression>(plus, std::make_shared<IntConstExpr>(3), MUL);
    auto rem = BinaryMathExpression(mul, std::make_shared<IntConstExpr>(7), REMDIV);

    auto result = rem.get_value(g);
    EXPECT_EQ(result->get_type(), Int);
    EXPECT_EQ(as_int(result), 6);
}

}
