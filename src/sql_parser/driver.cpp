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
	auto result = parse_ctx_.parse(query_io_.get_query());
	execute_queries(parse_ctx_.get_queries());
	print_error(result);
	shrink_queries(result);
    } while (query_io_.more_context_available() || !query_io_.query_empty());
}

void driver::shrink_queries(ParsingContext::ParsingResult result) {
    using Res = ParsingContext::ParsingResult;
    if(result == Res::Ok ||
       (result == Res::MoreContextRequired && !query_io_.more_context_available())) {
	return query_io_.clear_query();
    }
    query_io_.shrink_queries(parse_ctx_.parsed_queries());
}

void driver::print_error(ParsingContext::ParsingResult result) const {
    using Res = ParsingContext::ParsingResult;
    if(result == Res::Ok) return;
    auto error = parse_ctx_.get_error();
    if(!error) 
	throw std::logic_error("ParsingContext returned error without error itself");
    if(result == Res::Error || !query_io_.more_context_available())
	err_printer_.print_error(*error);
}

void driver::execute_queries(ParsingContext::Queries& queries) const {
    std::for_each(queries.begin(), queries.end(), [this](uptr<Query>& ptr) {
	ast_executor_.execute_sql_ast(std::move(ptr));
    });
    queries.clear();
}

void driver::reset_before_parse_process() {
    query_io_.reset();
    parse_ctx_.reset();
}

}
