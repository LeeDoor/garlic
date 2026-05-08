#include "binary_math_expression.hpp"
#include "unary_math_expression.hpp"
#include "constant_expression.hpp"
#include "table_value_expression.hpp"
#include "tables_gatherer_mock.hpp"

namespace garlic {

static const TablesGathered gatherers{};

static sptr<CellValue> unwrap_value(ExpectedCellValue result) {
    EXPECT_TRUE(result.has_value()) << result.error();
    return result ? *result : nullptr;
}

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
    auto lhs = std::make_shared<IntConstExpr>(17);
    auto rhs = std::make_shared<IntConstExpr>(5);

    EXPECT_EQ(as_int(unwrap_value(BinaryMathExpression(lhs, rhs, Add).resolve(gatherers))), 22);
    EXPECT_EQ(as_int(unwrap_value(BinaryMathExpression(lhs, rhs, Sub).resolve(gatherers))), 12);
    EXPECT_EQ(as_int(unwrap_value(BinaryMathExpression(lhs, rhs, Mul).resolve(gatherers))), 85);
    EXPECT_EQ(as_int(unwrap_value(BinaryMathExpression(lhs, rhs, Div).resolve(gatherers))), 3);
    EXPECT_EQ(as_int(unwrap_value(BinaryMathExpression(lhs, rhs, Remdiv).resolve(gatherers))), 2);
}

TEST(test_binary_math_expression, mixedIntFloatReturnsFloat) {
    auto lhs = std::make_shared<IntConstExpr>(5);
    auto rhs = std::make_shared<FloatConstExpr>(2.0f);

    auto add_res = BinaryMathExpression(lhs, rhs, Add).resolve(gatherers);
    auto div_res = BinaryMathExpression(lhs, rhs, Div).resolve(gatherers);
    auto rem_res = BinaryMathExpression(lhs, rhs, Remdiv).resolve(gatherers);

    ASSERT_TRUE(add_res.has_value()) << add_res.error();
    ASSERT_TRUE(div_res.has_value()) << div_res.error();
    ASSERT_TRUE(rem_res.has_value()) << rem_res.error();

    EXPECT_EQ((*add_res)->get_type(), Float);
    EXPECT_EQ((*div_res)->get_type(), Float);
    EXPECT_EQ((*rem_res)->get_type(), Float);
    EXPECT_FLOAT_EQ(as_float(*add_res), 7.0f);
    EXPECT_FLOAT_EQ(as_float(*div_res), 2.5f);
    EXPECT_FLOAT_EQ(as_float(*rem_res), 1.0f);
}

TEST(test_binary_math_expression, operandWithoutMathSupportShouldThrow) {
    auto lhs = std::make_shared<StringConstExpr>("abc");
    auto rhs = std::make_shared<IntConstExpr>(2);

    BinaryMathExpression expr(lhs, rhs, Add);
    EXPECT_THROW(std::ignore = expr.resolve(gatherers), std::logic_error);
}

TEST(test_unary_math_expression, absAndNegForInt) {
    auto val = std::make_shared<IntConstExpr>(-17);

    EXPECT_EQ(as_int(unwrap_value(UnaryMathExpression(val, Abs).resolve(gatherers))), 17);
    EXPECT_EQ(as_int(unwrap_value(UnaryMathExpression(val, Neg).resolve(gatherers))), 17);
}

TEST(test_unary_math_expression, absAndNegForFloat) {
    auto val = std::make_shared<FloatConstExpr>(-2.5f);

    EXPECT_FLOAT_EQ(as_float(unwrap_value(UnaryMathExpression(val, Abs).resolve(gatherers))), 2.5f);
    EXPECT_FLOAT_EQ(as_float(unwrap_value(UnaryMathExpression(val, Neg).resolve(gatherers))), 2.5f);
}

TEST(test_unary_math_expression, operandWithoutMathSupportShouldThrow) {
    auto val = std::make_shared<StringConstExpr>("abc");

    UnaryMathExpression expr(val, Neg);
    EXPECT_THROW(std::ignore = expr.resolve(gatherers), std::logic_error);
}

TEST(test_binary_unary_math_expression, composedBigExpression) {
    // ((|-(1000)| + 24) * 3) % 7 = 0
    auto base = std::make_shared<IntConstExpr>(1000);
    auto neg = std::make_shared<UnaryMathExpression>(base, Neg);
    auto abs = std::make_shared<UnaryMathExpression>(neg, Abs);
    auto plus = std::make_shared<BinaryMathExpression>(abs, std::make_shared<IntConstExpr>(24), Add);
    auto mul = std::make_shared<BinaryMathExpression>(plus, std::make_shared<IntConstExpr>(3), Mul);
    auto rem = BinaryMathExpression(mul, std::make_shared<IntConstExpr>(7), Remdiv);

    auto result = rem.resolve(gatherers);
    ASSERT_TRUE(result.has_value()) << result.error();
    EXPECT_EQ((*result)->get_type(), Int);
    EXPECT_EQ(as_int(*result), 6);
}

TEST(test_binary_math_expression, getUsedTables_MergesBothOperands) {
    auto lhs = std::make_shared<TableValueExpression>(TablesGathererMock{ Int }, "users", "age");
    auto rhs = std::make_shared<TableValueExpression>(TablesGathererMock{ Int }, "foods", "calories");

    EXPECT_EQ(
        BinaryMathExpression(lhs, rhs, Add).get_used_tables(),
        Expression::UsedTables({"users", "foods"})
    );
}

TEST(test_unary_math_expression, getUsedTables_ForwardsOperandTables) {
    auto operand = std::make_shared<TableValueExpression>(TablesGathererMock{ Int }, "users", "age");

    EXPECT_EQ(
        UnaryMathExpression(operand, Neg).get_used_tables(),
        Expression::UsedTables({"users"})
    );
}

}
