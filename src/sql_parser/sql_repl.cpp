#include "sql_repl.hpp"
#include "query_input.hpp"

namespace garlic::sql_parser {

SqlRepl::SqlRepl(bool debug_mode, QueryInput query_input, ErrorPrinter error_printer, SqlAstExecutor ast_executor)
: parse_ctx_{ debug_mode }
, query_input_{ std::move(query_input) }
, error_printer_{ std::move(error_printer) }
, ast_executor_{ std::move(ast_executor) }
{ }

void SqlRepl::parse() {
    do {
	query_input_.readline();
	auto queries_and_errors = parse_ctx_.parse(query_input_.get_query());
	handle_results(queries_and_errors);
	shrink_queries(queries_and_errors);
    } while (query_input_.is_more_context_available() || !query_input_.is_query_empty());
}

void SqlRepl::shrink_queries(const ParserEngine::ParsingResults& results) {
    if(!results.empty()) {
	auto& last = results.back();
	if(last.is_error() && last.as_error().more_context_required) {
	    if(results.size() >= 2) {
		const auto second_last = std::prev(std::prev(results.end()));
		query_input_.should_be_shrinked(second_last->get_end_position().get_characters());
	    }   
	    return;
	}
    }
    query_input_.clear_query();
}

void SqlRepl::handle_results(const ParserEngine::ParsingResults& results) const {
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
