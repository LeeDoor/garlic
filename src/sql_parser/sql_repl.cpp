#include "sql_repl.hpp"
#include "query_input.hpp"

namespace garlic::sql_parser {

SqlRepl::SqlRepl(bool debug_mode, QueryInput query_input, ErrorPrinter error_printer, SqlAstExecutor ast_executor)
: parse_ctx_{ debug_mode }
, query_input_{ std::move(query_input) }
, error_printer_{ std::move(error_printer) }
, ast_executor_{ std::move(ast_executor) }
{ }

void SqlRepl::run() {
    do {
	query_input_.readline();
	auto parse_session_result = parse_ctx_.parse(query_input_.get_query());
	handle_results(parse_session_result);
	query_input_.shrink_n_characters(parse_session_result.characters_parsed);
    } while (query_input_.is_more_context_available() || !query_input_.is_query_empty());
}

void SqlRepl::handle_results(const ParserEngine::Results& parse_results) const {
    const auto& results = parse_results.results;
    std::for_each(results.begin(), results.end(), [this](const ParsingResult& result) {
	if(result.is_query()) {
	    ast_executor_.execute_sql_ast(result.as_query());
	}
	else if(result.is_error()) {
	    print_error(result.as_error());
	}
    });
}

void SqlRepl::print_error(const ParsingError& error) const {
    if(!(error.more_context_required && query_input_.is_more_context_available()))
	error_printer_.print_error(error);
}

}
