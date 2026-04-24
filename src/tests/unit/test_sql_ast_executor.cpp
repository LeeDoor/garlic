#include "constant_expression.hpp"
#include "error_printer.hpp"
#include "select_query.hpp"
#include "sql_ast_executor.hpp"

namespace garlic {

class ThrowingExpressionForExecutorTest : public Expression {
public:
    ThrowingExpressionForExecutorTest() : Expression{ Int } {}

    ExpectedValid validate() const override {
        throw std::logic_error("Unexpected call");
    }

    ExpectedCellValue resolve(sptr<TableValueGatherer>) const override {
        return std::unexpected("expression evaluate failed");
    }
};

namespace {

class ScopedStreamRedirect {
public:
    ScopedStreamRedirect(std::ostream& stream, std::ostream& replacement)
    : stream_{ stream }
    , old_buffer_{ stream.rdbuf(replacement.rdbuf()) }
    {}

    ~ScopedStreamRedirect() {
        stream_.rdbuf(old_buffer_);
    }

private:
    std::ostream& stream_;
    std::streambuf* old_buffer_;
};

uptr<Query> make_query(uptr<Expression> expression) {
    SelectQuery::ColumnsContainer columns;
    columns.emplace_back(std::move(expression));
    return std::make_unique<SelectQuery>(std::move(columns));
}

std::string format_single_value_table(std::string_view column_name, std::string_view value) {
    const auto width = std::max(column_name.size(), value.size());
    const std::string border = "+" + std::string(width + 2, '-') + "+\n";
    return border
        + "| " + std::string(column_name) + std::string(width - column_name.size(), ' ') + " |\n"
        + border
        + "| " + std::string(value) + std::string(width - value.size(), ' ') + " |\n"
        + border;
}

}

TEST(test_sql_ast_executor, successfulQueryWritesOnlyToStdout) {
    std::ostringstream stdout_stream;
    std::ostringstream stderr_stream;
    ScopedStreamRedirect stdout_redirect(std::cout, stdout_stream);
    ScopedStreamRedirect stderr_redirect(std::cerr, stderr_stream);

    sql_parser::ErrorPrinter error_printer;
    sql_parser::SqlAstExecutor executor(error_printer);
    auto query = make_query(std::make_unique<IntConstExpr>(5));

    executor.execute_sql_ast(query);

    EXPECT_EQ(stdout_stream.str(), format_single_value_table("Int", "5"));
    EXPECT_TRUE(stderr_stream.str().empty());
}

TEST(test_sql_ast_executor, runtimeErrorWritesOnlyToStderr) {
    std::ostringstream stdout_stream;
    std::ostringstream stderr_stream;
    ScopedStreamRedirect stdout_redirect(std::cout, stdout_stream);
    ScopedStreamRedirect stderr_redirect(std::cerr, stderr_stream);

    sql_parser::ErrorPrinter error_printer;
    sql_parser::SqlAstExecutor executor(error_printer);
    auto query = make_query(std::make_unique<ThrowingExpressionForExecutorTest>());

    executor.execute_sql_ast(query);

    EXPECT_TRUE(stdout_stream.str().empty());
    EXPECT_EQ(stderr_stream.str(), "[RUNTIME_ERROR] expression evaluate failed\n\n");
}

}
