#include "select_query.hpp"
#include "query_result.hpp"
#include "constant_expression.hpp"
#include "condition_mock.hpp"
#include "typed_table.hpp"
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
    UsedTables get_used_tables() const override { return {}; }
};

class ThrowingExpression : public Expression {
public:
    ThrowingExpression() : Expression{ Int } {}
    ExpectedCellValue resolve(const TablesGathered& gatherers) const override {
        (void)gatherers;
        return std::unexpected("expression evaluate failed");
    }
    UsedTables get_used_tables() const override { return {}; }
};

class TestSelectorGenerator : public SelectorGenerator {
public:
    explicit TestSelectorGenerator(Selector&& selector) : selector_{ std::move(selector) } {}

    ExpectedOrStr<std::list<Selector>> generate(const TablesContainer&) override {
        return std::list<Selector>{ selector_ };
    }

    Expression::UsedTables get_used_tables() const override {
        return selector_.ast->get_used_tables();
    }

    bool requires_from_clause() const override {
        return false;
    }

private:
    Selector selector_;
};

class TestSelectQueries : public ::testing::Test {
protected:
    static TableValueGathererFactory make_unused_factory() {
        return TableValueGathererFactory{
            std::unordered_map<TableNameType, sptr<TypedTable>>{}
        };
    }

    static sptr<TypedTable> make_non_empty_table(std::initializer_list<IntType> values) {
        auto table = std::make_shared<TypedTable>(
            std::initializer_list<PublicColumnInfo>{
                PublicColumnInfo{ Int, "v", 0 },
            }
        );
        std::size_t row = 0;
        for(const auto value : values) {
            table->create_empty_row();
            table->set_value(row++, 0, value);
        }
        return table;
    }

    template<typename... Exprs>
    static SelectQuery make_query(Exprs&&... exprs) {
        SelectQuery::Selectors columns;
        (columns.emplace_back(std::forward<Exprs>(exprs)), ...);
        return SelectQuery(make_selector_generators(std::move(columns)));
    }

    static SelectQuery::SelectorGenerators make_selector_generators(SelectQuery::Selectors columns) {
        SelectQuery::SelectorGenerators selector_generators;
        for(auto& column : columns)
            selector_generators.push_back(std::make_unique<TestSelectorGenerator>(std::move(column)));
        return selector_generators;
    }

    static SelectQuery make_query_from_columns(SelectQuery::Selectors columns) {
        return SelectQuery(make_selector_generators(std::move(columns)));
    }

    static SelectQuery make_query_from_columns(
        SelectQuery::Selectors columns,
        SelectQuery::TablesContainer tables
    ) {
        return SelectQuery(make_selector_generators(std::move(columns)), std::move(tables));
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

    static std::vector<std::string_view> split_lines(std::string_view cell) {
        std::vector<std::string_view> lines;
        size_t line_start = 0;
        while(line_start <= cell.size()) {
            const auto newline_pos = cell.find('\n', line_start);
            if(newline_pos == std::string_view::npos) {
                lines.push_back(cell.substr(line_start));
                break;
            }

            lines.push_back(cell.substr(line_start, newline_pos - line_start));
            line_start = newline_pos + 1;
        }
        return lines;
    }

    static void append_separator(std::string& out, const std::vector<size_t>& widths) {
        out += '+';
        for(const auto width : widths) {
            out += std::string(width + 2, '-');
            out += '+';
        }
        out += '\n';
    }

    static void append_table_row(
        std::string& out,
        const std::vector<std::vector<std::string_view>>& cells,
        const std::vector<size_t>& widths
    ) {
        size_t height = 1;
        for(const auto& cell_lines : cells) {
            height = std::max(height, cell_lines.size());
        }

        for(size_t h = 0; h < height; ++h) {
            out += '|';
            for(size_t c = 0; c < cells.size(); ++c) {
                const auto current_line = h < cells[c].size() ? cells[c][h] : std::string_view{};
                const auto continuation = h + 1 < cells[c].size() ? '+' : ' ';
                out += ' ';
                out += std::string(current_line);
                out += std::string(widths[c] - current_line.size(), ' ');
                out += continuation;
                out += '|';
            }
            out += '\n';
        }
    }

    static std::string format_single_row_table(
        std::initializer_list<std::string_view> header_cells,
        std::initializer_list<std::string_view> body_cells
    ) {
        EXPECT_EQ(header_cells.size(), body_cells.size());

        std::vector<std::vector<std::string_view>> header_lines;
        std::vector<std::vector<std::string_view>> body_lines;
        std::vector<size_t> widths;
        header_lines.reserve(header_cells.size());
        body_lines.reserve(body_cells.size());
        widths.reserve(header_cells.size());

        auto body_it = body_cells.begin();
        for(const auto header_cell : header_cells) {
            auto header_split = split_lines(header_cell);
            auto body_split = split_lines(*body_it++);

            size_t width = 0;
            for(const auto line : header_split) {
                width = std::max(width, line.size());
            }
            for(const auto line : body_split) {
                width = std::max(width, line.size());
            }

            header_lines.push_back(std::move(header_split));
            body_lines.push_back(std::move(body_split));
            widths.push_back(width);
        }

        std::string out;
        append_separator(out, widths);
        append_table_row(out, header_lines, widths);
        append_separator(out, widths);
        append_table_row(out, body_lines, widths);
        append_separator(out, widths);
        return out;
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

TEST_F(TestSelectQueries, stringWithEscapedNewlinesFormatsAsMultilineTableRow) {
    auto factory = make_unused_factory();
    auto query = make_query(std::make_unique<StringConstExpr>("hello\nthere\nhi"));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    const auto expected = format_single_row_table({"String"}, {"hello\nthere\nhi"});
    EXPECT_EQ(std::string(result->format()), expected);
}

TEST_F(TestSelectQueries, tabsExpandToVisibleArrowSequenceInCells) {
    auto factory = make_unused_factory();
    SelectQuery::Selectors columns;
    columns.emplace_back(std::make_unique<StringConstExpr>("Alice\t"));
    columns.emplace_back(std::make_unique<IntConstExpr>(24));
    SelectQuery query = make_query_from_columns(std::move(columns));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    const std::string expected =
        "+-----------+-----+\n"
        "| String    | Int |\n"
        "+-----------+-----+\n"
        "| Alice ->  | 24  |\n"
        "+-----------+-----+\n";
    EXPECT_EQ(std::string(result->format()), expected);
}

TEST_F(TestSelectQueries, tabsAndNewlinesCombineInsideCells) {
    auto factory = make_unused_factory();
    SelectQuery::Selectors columns;
    columns.emplace_back(std::make_unique<StringConstExpr>("A\nB\tC"));
    columns.emplace_back(std::make_unique<IntConstExpr>(7));
    SelectQuery query = make_query_from_columns(std::move(columns));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    const std::string expected =
        "+--------+-----+\n"
        "| String | Int |\n"
        "+--------+-----+\n"
        "| A     +| 7   |\n"
        "| B -> C |     |\n"
        "+--------+-----+\n";
    EXPECT_EQ(std::string(result->format()), expected);
}

TEST_F(TestSelectQueries, mixedMultilineRowUsesTallestCellAndPadsShorterCells) {
    auto factory = make_unused_factory();
    SelectQuery::Selectors columns;
    columns.emplace_back("A", std::make_unique<StringConstExpr>("123\n456"));
    columns.emplace_back("B", std::make_unique<StringConstExpr>("1\n2\n3\n4"));
    columns.emplace_back(std::make_unique<IntConstExpr>(5));
    SelectQuery query = make_query_from_columns(std::move(columns));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    const auto expected = format_single_row_table({"A", "B", "Int"}, {"123\n456", "1\n2\n3\n4", "5"});
    EXPECT_EQ(std::string(result->format()), expected);
}

TEST_F(TestSelectQueries, fromClauseThreeTablesBuildsCartesianProduct) {
    TableValueGathererFactory factory{
        std::unordered_map<TableNameType, sptr<TypedTable>>{
            { "users", make_non_empty_table({1, 2}) },
            { "offices", make_non_empty_table({3, 4}) },
            { "foods", make_non_empty_table({5, 6}) },
        }
    };

    SelectQuery::Selectors columns;
    columns.emplace_back(std::make_unique<IntConstExpr>(1));
    SelectQuery::TablesContainer tables;
    tables.push_back({ "users" });
    tables.push_back({ "offices" });
    tables.push_back({ "foods" });
    SelectQuery query = make_query_from_columns(std::move(columns), std::move(tables));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);

    const std::string expected =
        "+-----+\n"
        "| Int |\n"
        "+-----+\n"
        "| 1   |\n"
        "| 1   |\n"
        "| 1   |\n"
        "| 1   |\n"
        "| 1   |\n"
        "| 1   |\n"
        "| 1   |\n"
        "| 1   |\n"
        "+-----+\n";
    EXPECT_EQ(std::string(result->format()), expected);
}

TEST_F(TestSelectQueries, expressionThrowingPropagatesException) {
    auto factory = make_unused_factory();
    auto query = make_query(std::make_unique<ThrowingExpression>());
    auto result = query.resolve(factory);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "expression evaluate failed");
}

TEST_F(TestSelectQueries, multilineHeaderUsesSameRulesAsBodyCells) {
    auto factory = make_unused_factory();
    SelectQuery::Selectors columns;
    columns.emplace_back("aboba\nmiddle\nlong name", std::make_unique<IntConstExpr>(12));
    SelectQuery query = make_query_from_columns(std::move(columns));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    const auto expected = format_single_row_table({"aboba\nmiddle\nlong name"}, {"12"});
    EXPECT_EQ(std::string(result->format()), expected);
}

TEST_F(TestSelectQueries, mixedMultilineHeadersUseTallestCellAndPadShorterCells) {
    auto factory = make_unused_factory();
    SelectQuery::Selectors columns;
    columns.emplace_back("A\nB", std::make_unique<IntConstExpr>(7));
    columns.emplace_back("1\n2\n3", std::make_unique<IntConstExpr>(8));
    columns.emplace_back("Tail", std::make_unique<IntConstExpr>(9));
    SelectQuery query = make_query_from_columns(std::move(columns));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    const auto expected = format_single_row_table({"A\nB", "1\n2\n3", "Tail"}, {"7", "8", "9"});
    EXPECT_EQ(std::string(result->format()), expected);
}

TEST_F(TestSelectQueries, multilineHeadersAndBodiesFormatIndependently) {
    auto factory = make_unused_factory();
    SelectQuery::Selectors columns;
    columns.emplace_back("A\nB", std::make_unique<StringConstExpr>("123\n456"));
    columns.emplace_back("C\nD\nE", std::make_unique<StringConstExpr>("1\n2\n3\n4"));
    columns.emplace_back("Int", std::make_unique<IntConstExpr>(5));
    SelectQuery query = make_query_from_columns(std::move(columns));

    auto result = unwrap_query_result(query.resolve(factory));
    ASSERT_NE(result, nullptr);
    const auto expected = format_single_row_table({"A\nB", "C\nD\nE", "Int"}, {"123\n456", "1\n2\n3\n4", "5"});
    EXPECT_EQ(std::string(result->format()), expected);
}

}
