#include "sql_repl.hpp"

namespace garlic::sql_parser {

SqlRepl::SqlRepl(bool debug_mode)
: parse_ctx_{ debug_mode }
{ }

void SqlRepl::parse() {
    reset_before_parse_process();
    do {
	query_io_.readline();
	auto queries_and_errors = parse_ctx_.parse(query_io_.get_query());
	handle_results(queries_and_errors);
	shrink_queries(queries_and_errors);
    } while (query_io_.is_more_context_available() || !query_io_.is_query_empty());
}

void SqlRepl::shrink_queries(const ParsingContext::ParsingResults& results) {
    if(!results.empty()) {
	auto& last = results.back();
	if(last.is_error() && last.as_error().more_context_required) {
	    if(results.size() >= 2) {
		const auto second_last = std::prev(std::prev(results.end()));
		query_io_.should_be_shrinked(second_last->get_end_position().get_characters());
	    }   
	    return;
	}
    }
    query_io_.clear_query();
}

void SqlRepl::handle_results(const ParsingContext::ParsingResults& results) const {
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
    if(!(error.more_context_required && query_io_.is_more_context_available()))
	err_printer_.print_error(error);
}

void SqlRepl::reset_before_parse_process() {
    query_io_.reset();
}

}
