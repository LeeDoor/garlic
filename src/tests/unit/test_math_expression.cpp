#include "cell_int_value.hpp"
#include "cell_float_value.hpp"
#include "cell_string_view_value.hpp"

namespace garlic {

static sptr<CellValue> unwrap_value(ExpectedCellValue value) {
    EXPECT_TRUE(value.has_value()) << value.error();
    return value ? *value : nullptr;
}

static sptr<CellAcceptMathOp> must_math(ExpectedCellValue value) {
    auto math = std::dynamic_pointer_cast<CellAcceptMathOp>(unwrap_value(std::move(value)));
    EXPECT_NE(math, nullptr);
    return math;
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

TEST(test_math_expression, simpleIntMath) {
    auto a = std::make_shared<CellIntValue>(9);
    auto b = std::make_shared<CellIntValue>(4);

    EXPECT_EQ(as_int(unwrap_value(a->add(b))), 13);
    EXPECT_EQ(as_int(unwrap_value(a->sub(b))), 5);
    EXPECT_EQ(as_int(unwrap_value(a->mul(b))), 36);
    EXPECT_EQ(as_int(unwrap_value(a->div(b))), 2);
}

TEST(test_math_expression, simpleFloatMath) {
    auto a = std::make_shared<CellFloatValue>(9.5f);
    auto b = std::make_shared<CellFloatValue>(4.0f);

    EXPECT_FLOAT_EQ(as_float(unwrap_value(a->add(b))), 13.5f);
    EXPECT_FLOAT_EQ(as_float(unwrap_value(a->sub(b))), 5.5f);
    EXPECT_FLOAT_EQ(as_float(unwrap_value(a->mul(b))), 38.0f);
    EXPECT_FLOAT_EQ(as_float(unwrap_value(a->div(b))), 2.375f);
}

TEST(test_math_expression, intDivisionShouldTruncate) {
    auto five = std::make_shared<CellIntValue>(5);
    auto six = std::make_shared<CellIntValue>(6);

    auto result = five->div(six);
    ASSERT_TRUE(result.has_value()) << result.error();
    EXPECT_EQ((*result)->get_type(), Int);
    EXPECT_EQ(as_int(*result), 0);
}

TEST(test_math_expression, mixedIntAndFloatShouldReturnFloat) {
    auto i = std::make_shared<CellIntValue>(5);
    auto f = std::make_shared<CellFloatValue>(2.0f);

    auto i_plus_f = i->add(f);
    auto f_plus_i = f->add(i);
    auto i_div_f = i->div(f);

    ASSERT_TRUE(i_plus_f.has_value()) << i_plus_f.error();
    ASSERT_TRUE(f_plus_i.has_value()) << f_plus_i.error();
    ASSERT_TRUE(i_div_f.has_value()) << i_div_f.error();

    EXPECT_EQ((*i_plus_f)->get_type(), Float);
    EXPECT_EQ((*f_plus_i)->get_type(), Float);
    EXPECT_EQ((*i_div_f)->get_type(), Float);

    EXPECT_FLOAT_EQ(as_float(*i_plus_f), 7.0f);
    EXPECT_FLOAT_EQ(as_float(*f_plus_i), 7.0f);
    EXPECT_FLOAT_EQ(as_float(*i_div_f), 2.5f);
}

TEST(test_math_expression, bigIntegerExpression) {
    auto v1 = std::make_shared<CellIntValue>(1000000);
    auto v2 = std::make_shared<CellIntValue>(2000000);
    auto v3 = std::make_shared<CellIntValue>(3);
    auto v4 = std::make_shared<CellIntValue>(4000000);
    auto v5 = std::make_shared<CellIntValue>(7);

    auto result = must_math(
        must_math(
            must_math(v1->add(v2))->mul(v3)
        )->sub(v4)
    )->div(v5);
    ASSERT_TRUE(result.has_value()) << result.error();
    EXPECT_EQ((*result)->get_type(), Int);
    EXPECT_EQ(as_int(*result), 714285);
}

TEST(test_math_expression, bigFloatExpression) {
    auto v1 = std::make_shared<CellFloatValue>(1.5f);
    auto v2 = std::make_shared<CellIntValue>(2);
    auto v3 = std::make_shared<CellIntValue>(10);
    auto v4 = std::make_shared<CellIntValue>(3);
    auto v5 = std::make_shared<CellIntValue>(7);

    auto result = must_math(
        must_math(
            must_math(v1->add(v2))->mul(v3)
        )->sub(v4)
    )->div(v5);
    ASSERT_TRUE(result.has_value()) << result.error();
    EXPECT_EQ((*result)->get_type(), Float);
    EXPECT_NEAR(as_float(*result), 32.0f / 7.0f, 1e-6f);
}

TEST(test_math_expression, intRemainderShouldBehaveAsModulo) {
    auto a = std::make_shared<CellIntValue>(17);
    auto b = std::make_shared<CellIntValue>(5);

    auto result = a->remdiv(b);
    ASSERT_TRUE(result.has_value()) << result.error();
    EXPECT_EQ((*result)->get_type(), Int);
    EXPECT_EQ(as_int(*result), 2);
}

TEST(test_math_expression, arithmeticWithStringShouldThrow) {
    auto i = std::make_shared<CellIntValue>(5);
    auto f = std::make_shared<CellFloatValue>(5.0f);
    auto s = std::make_shared<CellStringViewValue>("abc");

    EXPECT_THROW(i->add(s), std::logic_error);
    EXPECT_THROW(i->sub(s), std::logic_error);
    EXPECT_THROW(i->mul(s), std::logic_error);
    EXPECT_THROW(i->div(s), std::logic_error);
    EXPECT_THROW(i->remdiv(s), std::logic_error);

    EXPECT_THROW(f->add(s), std::logic_error);
    EXPECT_THROW(f->sub(s), std::logic_error);
    EXPECT_THROW(f->mul(s), std::logic_error);
    EXPECT_THROW(f->div(s), std::logic_error);
    EXPECT_THROW(f->remdiv(s), std::logic_error);
}

}
