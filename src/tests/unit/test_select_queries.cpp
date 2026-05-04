#include "select_query.hpp"
#include "query_result.hpp"
#include "constant_expression.hpp"
#include "condition_mock.hpp"
#include <limits>
#include <sstream>

namespace garlic {

class ThrowingCondition : public Condition {
public:
    ThrowingCondition() : Condition{ Boolean } {}
    ExpectedCellBooleanValue resolve_bool(const TablesGathered& gatherers) const override {
        (void)gatherers;
        return std::unexpected("condition resolve failed");
    }
};

class ThrowingExpression : public Expression {
public:
    ThrowingExpression() : Expression{ Int } {}
    ExpectedCellValue resolve(const TablesGathered& gatherers) const override {
        (void)gatherers;
        return std::unexpected("expression evaluate failed");
    }
};

class TestSelectQueries : public ::testing::Test {
protected:
    static TableValueGathererFactory make_unused_factory() {
        return TableValueGathererFactory{
            std::unordered_map<TableNameType, sptr<TypedTable>>{}
        };
    }

    template<typename... Exprs>
    static SelectQuery make_query(Exprs&&... exprs) {
        SelectQuery::ColumnsContainer columns;
        (columns.emplace_back(std::forward<Exprs>(exprs)), ...);
        return SelectQuery(std::move(columns));
    }

    static sptr<QueryResult> unwrap_query_result(Query::ExpectedQueryResult result) {
        EXPECT_TRUE(result.has_value()) << result.error();
        if(!result)
            return nullptr;
        return *result;
    }

    static std::string format_single_value_table(std::string_view column_name, std::string_view value) {
        const auto width = std::max(column_name.size(), value.size());
        const std::string border = "+" + std::string(width + 2, '-') + "+\n";
        return border
            + "| " + std::string(column_name) + std::string(width - column_name.size(), ' ') + " |\n"
            + border
            + "| " + std::string(value) + std::string(width - value.size(), ' ') + " |\n"
            + border;
    }

    static std::string format_single_value_table_with_raw_header(std::string_view raw_header, std::string_view value) {
        const auto escape = std::find_if(
            raw_header.begin(),
            raw_header.end(),
            [](char ch) { return std::iscntrl(static_cast<unsigned char>(ch)) != 0; }
        );
        std::string header;
        if (escape == raw_header.end()) {
            header.assign(raw_header.begin(), raw_header.end());
        } else {
            header.assign(raw_header.begin(), escape);
            header += '+';
        }
        return format_single_value_table(header, value);
    }

    static std::string format_float_like_query(FloatType value) {
        std::ostringstream os;
        os << value;
        return os.str();
    }
};

TEST_F(TestSelectQueries, conditionTrueFormatsAsOne) {
    auto factory = make_unused_factory();
    auto query = make_query(std::make_unique<ConditionMock>(true));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->format(), format_single_value_table("Boolean", "true"));
}

TEST_F(TestSelectQueries, conditionFalseFormatsAsZero) {
    auto factory = make_unused_factory();
    auto query = make_query(std::make_unique<ConditionMock>(false));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->format(), format_single_value_table("Boolean", "false"));
}

TEST_F(TestSelectQueries, conditionThrowingPropagatesException) {
    auto factory = make_unused_factory();
    auto query = make_query(std::make_unique<ThrowingCondition>());
    auto result = query.resolve(factory);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "condition resolve failed");
}

TEST_F(TestSelectQueries, expressionIntFormatsBasicNumber) {
    auto factory = make_unused_factory();
    auto query = make_query(std::make_unique<IntConstExpr>(42));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->format(), format_single_value_table("Int", "42"));
}

TEST_F(TestSelectQueries, expressionIntFormatsBoundaryNumbers) {
    auto factory = make_unused_factory();
    auto max_q = make_query(std::make_unique<IntConstExpr>(std::numeric_limits<IntType>::max()));
    auto min_q = make_query(std::make_unique<IntConstExpr>(std::numeric_limits<IntType>::min()));
    auto max_res = unwrap_query_result(max_q.resolve(factory));
    auto min_res = unwrap_query_result(min_q.resolve(factory));

    EXPECT_EQ(
        max_res->format(),
        format_single_value_table("Int", std::to_string(std::numeric_limits<IntType>::max()))
    );
    EXPECT_EQ(
        min_res->format(),
        format_single_value_table("Int", std::to_string(std::numeric_limits<IntType>::min()))
    );
}

TEST_F(TestSelectQueries, expressionFloatFormatsBasicNumber) {
    auto factory = make_unused_factory();
    auto query = make_query(std::make_unique<FloatConstExpr>(1.25f));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->format(), format_single_value_table("Float", "1.25"));
}

TEST_F(TestSelectQueries, expressionFloatFormatsSpecialValues) {
    auto factory = make_unused_factory();
    const std::array<FloatType, 6> values{
        std::numeric_limits<FloatType>::max(),
        std::numeric_limits<FloatType>::lowest(),
        std::numeric_limits<FloatType>::min(),
        std::numeric_limits<FloatType>::denorm_min(),
        std::numeric_limits<FloatType>::infinity(),
        std::numeric_limits<FloatType>::quiet_NaN(),
    };

    for(const auto v : values) {
        auto query = make_query(std::make_unique<FloatConstExpr>(v));
        auto result = unwrap_query_result(query.resolve(factory));
        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->format(), format_single_value_table("Float", format_float_like_query(v)));
    }
}

TEST_F(TestSelectQueries, stringWithEscapedNewlinesFormatAsEscapedTextInTable) {
    auto factory = make_unused_factory();
    auto query = make_query(std::make_unique<StringConstExpr>("hello\nthere\nhi"));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(
        result->format(),
        "+------------------+\n"
        "| String           |\n"
        "+------------------+\n"
        "| hello\\nthere\\nhi |\n"
        "+------------------+"
    );
}

TEST_F(TestSelectQueries, expressionThrowingPropagatesException) {
    auto factory = make_unused_factory();
    auto query = make_query(std::make_unique<ThrowingExpression>());
    auto result = query.resolve(factory);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "expression evaluate failed");
}

TEST_F(TestSelectQueries, escapedAliasHeaderShrinksBeforeTableFormatting) {
    auto factory = make_unused_factory();
    SelectQuery::ColumnsContainer columns;
    columns.emplace_back("aboba\n\t\nlong name", std::make_unique<IntConstExpr>(12));
    SelectQuery query(std::move(columns));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(
        result->format(),
        format_single_value_table_with_raw_header("aboba\n\t\nlong name", "12")
    );
}

}
