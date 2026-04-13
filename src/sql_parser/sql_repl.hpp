#pragma once
#include "error_printer.hpp"
#include "parser_engine.hpp"
#include "query_io.hpp"
#include "sql_ast_executor.hpp"

namespace garlic::sql_parser {

class SqlRepl {
public:
    explicit SqlRepl(bool debug_mode = false);

    void parse();
private:
    void print_error(const ParsingError& error) const;
    void reset_before_parse_process();
    void handle_results(const ParserEngine::ParsingResults& results) const;
    void shrink_queries(const ParserEngine::ParsingResults& results);

    ParserEngine parse_ctx_;
    QueryIO query_io_ {};
    ErrorPrinter err_printer_ {};
    SqlAstExecutor ast_executor_ {};
};

}
