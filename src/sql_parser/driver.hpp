#pragma once
#include "error_printer.hpp"
#include "parsing_context.hpp"
#include "query_io.hpp"
#include "sql_ast_executor.hpp"

namespace garlic::sql_parser {

class driver {
public:
    explicit driver(bool debug_mode = false);

    void parse();
private:
    void print_error(const ParsingError& error) const;
    void reset_before_parse_process();
    void handle_results(const ParsingContext::ParsingResults& results) const;
    void shrink_queries(const ParsingContext::ParsingResults& results);

    ParsingContext parse_ctx_;
    QueryIO query_io_ {};
    ErrorPrinter err_printer_ {};
    SqlAstExecutor ast_executor_ {};
};

}
