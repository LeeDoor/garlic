#include "driver.hpp"
#include "parser.tab.hpp"

namespace garlic::sql_parser {

driver::driver(bool debug_mode)
: parse_ctx_{ debug_mode }
{ }

void driver::parse() {
    reset_before_parse_process();
    do {
	query_io_.readline();
	auto& queries_and_errors = parse_ctx_.parse(query_io_.get_query());
	handle_results(queries_and_errors);
	shrink_queries(queries_and_errors);
    } while (query_io_.is_more_context_available() || !query_io_.is_query_empty());
}

void driver::shrink_queries(const ParsingContext::ParsingResults& results) {
    if(!results.empty()) {
	auto& last_result = results.back();
	const ParsingError *error = std::get_if<ParsingError>(&last_result);
	if(error && error->more_context_required) {
	    query_io_.shrink_to_last_query();
	    return;
	}
    }

    return query_io_.clear_query();
}

void driver::print_error(const ParsingError& error) const {
    if(!(error.more_context_required && query_io_.is_more_context_available()))
	err_printer_.print_error(error);
}

void driver::handle_results(ParsingContext::ParsingResults& results) const {
    std::for_each(results.begin(), results.end(), [this](ParsingContext::ParsingResult& result) {
	if(auto query_ptr = std::get_if<uptr<Query>>(&result)) {
	    ast_executor_.execute_sql_ast(std::move(*query_ptr));
	}
	else if(auto error = std::get_if<ParsingError>(&result)) {
	    print_error(*error);
	} else {
	    throw std::logic_error("incoming std::variant couldn't be resolved");
	}
    });
}

void driver::reset_before_parse_process() {
    query_io_.reset();
    parse_ctx_.reset_context();
}

}
