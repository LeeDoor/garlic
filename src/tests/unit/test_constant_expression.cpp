#include "constant_expression.hpp"
#include "table_value_gatherer_mock.hpp"

namespace garlic {

auto gatherer_mock = std::make_shared<::testing::StrictMock<TableValueGathererMock>>();

static sptr<CellComparable> as_comparable(Expression::ExpectedCellValue result) {
    EXPECT_TRUE(result.has_value()) << result.error();
    if(!result) return nullptr;
    auto value = std::dynamic_pointer_cast<CellComparable>(*result);
    EXPECT_NE(value, nullptr);
    return value;
}

TEST(test_constant_expression, IntBasicMethodKit) {
    sptr<Expression> expr_int1 = std::make_shared<IntConstExpr>(5);
    sptr<Expression> expr_int2 = std::make_shared<IntConstExpr>(5);
    sptr<Expression> expr_int3 = std::make_shared<IntConstExpr>(INT_MAX);

    auto value1 = as_comparable(expr_int1->get_value(gatherer_mock));
    auto value2 = as_comparable(expr_int2->get_value(gatherer_mock));
    auto value3 = as_comparable(expr_int3->get_value(gatherer_mock));

    EXPECT_EQ(value1->get_type(), CellType::Int);
    EXPECT_TRUE(value1->equals(value2));
    EXPECT_FALSE(value1->equals(value3));
}

TEST(test_constant_expression, FloatBasicMethodKit) {
    sptr<Expression> expr_float1 = std::make_shared<FloatConstExpr>(M_PI);
    sptr<Expression> expr_float2 = std::make_shared<FloatConstExpr>(M_PI);
    sptr<Expression> expr_float3 = std::make_shared<FloatConstExpr>(M_E);

    auto value1 = as_comparable(expr_float1->get_value(gatherer_mock));
    auto value2 = as_comparable(expr_float2->get_value(gatherer_mock));
    auto value3 = as_comparable(expr_float3->get_value(gatherer_mock));

    EXPECT_EQ(value1->get_type(), CellType::Float);
    EXPECT_TRUE(value1->equals(value2));
    EXPECT_FALSE(value1->equals(value3));
}

TEST(test_constant_expression, StringBasicMethodKit) {
    using namespace std::literals;
    sptr<Expression> expr_string1 = std::make_shared<StringConstExpr>("Hello"s);
    sptr<Expression> expr_string2 = std::make_shared<StringConstExpr>("Hello"s);
    sptr<Expression> expr_string3 = std::make_shared<StringConstExpr>("World"s);

    auto value1 = as_comparable(expr_string1->get_value(gatherer_mock));
    auto value2 = as_comparable(expr_string2->get_value(gatherer_mock));
    auto value3 = as_comparable(expr_string3->get_value(gatherer_mock));

    EXPECT_EQ(value1->get_type(), CellType::String);
    EXPECT_TRUE(value1->equals(value2));
    EXPECT_FALSE(value1->equals(value3));
}

}
