#include "select_query.hpp"
#include "constant_expression.hpp"
#include "condition_mock.hpp"
#include "table_value_gatherer_mock.hpp"
#include <limits>
#include <sstream>

namespace garlic {

class ThrowingCondition : public Condition {
public:
    ThrowingCondition() : Condition{ Boolean } {}
    ExpectedValid validate() const override { 
	throw std::logic_error("Unexpected call"); 
    }
    ExpectedCellBooleanValue resolve_bool(sptr<TableValueGatherer>) const override {
        return std::unexpected("condition resolve failed");
    }
};

class ThrowingExpression : public Expression {
public:
    ThrowingExpression() : Expression{ Int } {}
    ExpectedValid validate() const override { 
	throw std::logic_error("Unexpected call"); 
    }
    ExpectedCellValue resolve(sptr<TableValueGatherer>) const override {
        return std::unexpected("expression evaluate failed");
    }
};

class TestSelectQueries : public ::testing::Test {
protected:
    sptr<testing::StrictMock<TableValueGathererMock>> gatherer_ =
        std::make_shared<testing::StrictMock<TableValueGathererMock>>();

    static std::string format_single_value_table(std::string_view column_name, std::string_view value) {
        return std::string(column_name) + "\n" + std::string(value);
    }

    static std::string format_multi_value_table(
        std::initializer_list<std::string_view> column_names,
        std::initializer_list<std::string_view> values
    ) {
        std::ostringstream os;

        bool first = true;
        for(auto column_name : column_names) {
            if(!first) os << '\t';
            first = false;
            os << column_name;
        }
        os << '\n';

        first = true;
        for(auto value : values) {
            if(!first) os << '\t';
            first = false;
            os << value;
        }
        return os.str();
    }

    static std::string format_float_like_query(FloatType value) {
        std::ostringstream os;
        os << value;
        return os.str();
    }
};

TEST_F(TestSelectQueries, conditionTrueFormatsAsOne) {
    SelectQuery query({ SelectColumn{ std::make_shared<ConditionMock>(true) } });

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(result->format(), format_single_value_table("Boolean", "true"));
}

TEST_F(TestSelectQueries, conditionFalseFormatsAsZero) {
    SelectQuery query({ SelectColumn{ std::make_shared<ConditionMock>(false) } });

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(result->format(), format_single_value_table("Boolean", "false"));
}

TEST_F(TestSelectQueries, conditionThrowingPropagatesException) {
    SelectQuery query({ SelectColumn{ std::make_shared<ThrowingCondition>() } });
    EXPECT_EQ(query.resolve(gatherer_)->format(), "[RUNTIME_ERROR] condition resolve failed");
}

TEST_F(TestSelectQueries, expressionIntFormatsBasicNumber) {
    SelectQuery query({ SelectColumn{ std::make_shared<IntConstExpr>(42) } });

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(result->format(), format_single_value_table("Int", "42"));
}

TEST_F(TestSelectQueries, expressionIntFormatsBoundaryNumbers) {
    SelectQuery max_q({ SelectColumn{ std::make_shared<IntConstExpr>(std::numeric_limits<IntType>::max()) } });
    SelectQuery min_q({ SelectColumn{ std::make_shared<IntConstExpr>(std::numeric_limits<IntType>::min()) } });

    EXPECT_EQ(
        max_q.resolve(gatherer_)->format(),
        format_single_value_table("Int", std::to_string(std::numeric_limits<IntType>::max()))
    );
    EXPECT_EQ(
        min_q.resolve(gatherer_)->format(),
        format_single_value_table("Int", std::to_string(std::numeric_limits<IntType>::min()))
    );
}

TEST_F(TestSelectQueries, expressionFloatFormatsBasicNumber) {
    SelectQuery query({ SelectColumn{ std::make_shared<FloatConstExpr>(1.25f) } });

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(result->format(), format_single_value_table("Float", "1.25"));
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
        SelectQuery query({ SelectColumn{ std::make_shared<FloatConstExpr>(v) } });
        auto result = query.resolve(gatherer_);
        EXPECT_EQ(result->format(), format_single_value_table("Float", format_float_like_query(v)));
    }
}

TEST_F(TestSelectQueries, expressionStringFormatsVeryLargeValue) {
    StringType huge(1 << 20, 'X');
    huge.replace(0, 5, "BEGIN");
    huge.replace(huge.size() - 3, 3, "END");
    SelectQuery query({ SelectColumn{ std::make_shared<StringConstExpr>(huge) } });

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(result->format(), format_single_value_table("String", huge));
}

TEST_F(TestSelectQueries, expressionThrowingPropagatesException) {
    SelectQuery query({ SelectColumn{ std::make_shared<ThrowingExpression>() } });
    EXPECT_EQ(query.resolve(gatherer_)->format(), "[RUNTIME_ERROR] expression evaluate failed");
}

TEST_F(TestSelectQueries, multipleColumnsFormatsMixedTypes) {
    SelectQuery query({
        SelectColumn{ std::make_shared<FloatConstExpr>(5.5f) },
        SelectColumn{ std::make_shared<IntConstExpr>(1) },
        SelectColumn{ std::make_shared<StringConstExpr>("hello") },
        SelectColumn{ std::make_shared<ConditionMock>(true) },
    });

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(
        result->format(),
        format_multi_value_table(
            { "Float", "Int", "String", "Boolean" },
            { "5.5", "1", "hello", "true" }
        )
    );
}

TEST_F(TestSelectQueries, appendColumnBuildsMultiSelectorQuery) {
    SelectQuery query;
    query.append_column(std::make_shared<IntConstExpr>(7));
    query.append_column(std::make_shared<StringConstExpr>("value"));
    query.append_column(std::make_shared<ConditionMock>(false));

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(
        result->format(),
        format_multi_value_table(
            { "Int", "String", "Boolean" },
            { "7", "value", "false" }
        )
    );
}

TEST_F(TestSelectQueries, manyColumnsFormatAsSingleRowTable) {
    SelectQuery query;
    std::vector<std::string> expected_numbers;
    std::ostringstream expected_types;
    std::ostringstream expected_values;

    for(int i = 1; i <= 60; ++i) {
        query.append_column(std::make_shared<IntConstExpr>(i));
        expected_numbers.push_back(std::to_string(i));

        if(i != 1) {
            expected_types << '\t';
            expected_values << '\t';
        }
        expected_types << "Int";
        expected_values << i;
    }

    auto result = query.resolve(gatherer_);
    EXPECT_EQ(result->format(), expected_types.str() + "\n" + expected_values.str());
}

}
