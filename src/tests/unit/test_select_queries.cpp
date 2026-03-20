#include "condition_select_query.hpp"
#include "expression_select_query.hpp"
#include "query_result.hpp"
#include "constant_expression.hpp"
#include "condition_mock.hpp"
#include "table_value_gatherer_mock.hpp"
#include <limits>
#include <sstream>

namespace garlic {

class ThrowingCondition : public Condition {
public:
    bool resolve(sptr<TableValueGatherer>) const override {
        throw std::logic_error("condition resolve failed");
    }
};

class ThrowingExpression : public Expression {
public:
    sptr<CellValue> get_value(sptr<TableValueGatherer>) const override {
        throw std::logic_error("expression evaluate failed");
    }
};

class TestSelectQueries : public ::testing::Test {
protected:
    sptr<testing::StrictMock<TableValueGathererMock>> gatherer_ =
        std::make_shared<testing::StrictMock<TableValueGathererMock>>();

    static std::string format_float_like_query(FloatType value) {
        std::ostringstream os;
        os << value;
        return os.str();
    }
};

TEST_F(TestSelectQueries, conditionTrueFormatsAsOne) {
    ConditionSelectQuery query(std::make_unique<ConditionMock>(true));

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(result->format(), "1");
}

TEST_F(TestSelectQueries, conditionFalseFormatsAsZero) {
    ConditionSelectQuery query(std::make_unique<ConditionMock>(false));

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(result->format(), "0");
}

TEST_F(TestSelectQueries, conditionThrowingPropagatesException) {
    ConditionSelectQuery query(std::make_unique<ThrowingCondition>());

    EXPECT_THROW(query.resolve(gatherer_), std::logic_error);
}

TEST_F(TestSelectQueries, expressionIntFormatsBasicNumber) {
    ExpressionSelectQuery query(std::make_unique<IntConstExpr>(42));

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(result->format(), "42");
}

TEST_F(TestSelectQueries, expressionIntFormatsBoundaryNumbers) {
    ExpressionSelectQuery max_q(std::make_unique<IntConstExpr>(std::numeric_limits<IntType>::max()));
    ExpressionSelectQuery min_q(std::make_unique<IntConstExpr>(std::numeric_limits<IntType>::min()));

    EXPECT_EQ(max_q.resolve(gatherer_)->format(), std::to_string(std::numeric_limits<IntType>::max()));
    EXPECT_EQ(min_q.resolve(gatherer_)->format(), std::to_string(std::numeric_limits<IntType>::min()));
}

TEST_F(TestSelectQueries, expressionFloatFormatsBasicNumber) {
    ExpressionSelectQuery query(std::make_unique<FloatConstExpr>(1.25f));

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(result->format(), "1.25");
}

TEST_F(TestSelectQueries, expressionFloatFormatsSpecialValues) {
    const std::array<FloatType, 6> values{
        std::numeric_limits<FloatType>::max(),
        std::numeric_limits<FloatType>::lowest(),
        std::numeric_limits<FloatType>::min(),
        std::numeric_limits<FloatType>::denorm_min(),
        std::numeric_limits<FloatType>::infinity(),
        std::numeric_limits<FloatType>::quiet_NaN(),
    };

    for(const auto v : values) {
        ExpressionSelectQuery query(std::make_unique<FloatConstExpr>(v));
        auto result = query.resolve(gatherer_);
        EXPECT_EQ(result->format(), format_float_like_query(v));
    }
}

TEST_F(TestSelectQueries, expressionStringFormatsVeryLargeValue) {
    StringType huge(1 << 20, 'X');
    huge.replace(0, 5, "BEGIN");
    huge.replace(huge.size() - 3, 3, "END");
    ExpressionSelectQuery query(std::make_unique<StringConstExpr>(huge));

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(result->format().size(), huge.size());
    EXPECT_TRUE(std::ranges::equal(result->format(), huge));
}

TEST_F(TestSelectQueries, expressionThrowingPropagatesException) {
    ExpressionSelectQuery query(std::make_unique<ThrowingExpression>());

    EXPECT_THROW(query.resolve(gatherer_), std::logic_error);
}

}
