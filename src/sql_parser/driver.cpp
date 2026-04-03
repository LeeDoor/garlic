#include "driver.hpp"
#include "manual_io.hpp"
#include "parser.tab.hpp"

namespace garlic::sql_parser {

driver::driver(bool debug_mode)
: debug_mode_{ debug_mode }
{ }

void driver::invoke_error(ErrorStage stage, const std::string& err) {
    if(!(is_eof_ && query_io_.more_context_available())) {
	query_io_.print_error(stage, err, token_begin_location_);
	query_executed();
    }
}
void driver::query_executed() {
    ++executed_queries_;
    if(is_manual_IO()) {
	current_query_beginning_.initialize();
	location_.initialize();
    } else {
	current_query_beginning_ = location_;
    }
}

void driver::met_eof() {
    is_eof_ = true;
}
void driver::memorize_token_begin_loc() {
    token_begin_location_ = location_;
}
void driver::parse() {
    reset_before_parse_process();
    do {
	if(is_eof_) {
	    query_io_.readline();
	}
	reset_before_parsing_iteration();
	parse_repl();
	query_io_.shrink_queries(executed_queries_);
    } while (query_io_.more_context_available() || !query_io_.query_empty());
}

void driver::reset_before_parse_process() {
    query_io_.reset();
}

void driver::reset_before_parsing_iteration() {
    is_eof_ = false;
    location_ = current_query_beginning_;
    executed_queries_ = 0;
}

void driver::parse_repl() {
    yy::parser parse(*this);
    parse.set_debug_level(debug_mode_);
    scan_begin();
    if(parse() == 0)
	query_executed();
    scan_end();
}

}
