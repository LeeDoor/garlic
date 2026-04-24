#pragma once
#include "error_printer.hpp"
#include "parser_engine.hpp"
#include "query_input.hpp"
#include "sql_ast_executor.hpp"

namespace garlic::sql_parser {

class SqlRepl {
public:
    explicit SqlRepl(
	bool debug_mode, 
	QueryInput query_input, 
	ErrorPrinter error_printer,
	SqlAstExecutor ast_executor);

    void run();
private:
    void print_error(const ParsingError& error) const;
    void handle_results(const ParserEngine::Results& results) const;

    ParserEngine parse_ctx_;
    QueryInput query_input_;
    ErrorPrinter error_printer_;
    SqlAstExecutor ast_executor_;
};

}
