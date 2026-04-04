#include "driver.hpp"
#include "manual_io.hpp"
#include "parser.tab.hpp"

namespace garlic::sql_parser {

driver::driver(bool debug_mode)
: debug_mode_{ debug_mode }
{ }

void driver::invoke_error(ErrorStage stage, const std::string& err) {
    if(!(more_context_required_ && query_io_.more_context_available())) {
	query_io_.print_error(stage, err, location_.token_start());
	query_executed();
    }
}
void driver::query_executed() {
    ++executed_queries_;
    if(is_manual_IO()) {
	location_.reset();
    } else {
	location_.on_query_start();
    }
}

void driver::met_eof() {
    more_context_required_ = true;
}
void driver::memorize_token_begin_loc() {
    location_.on_token_start();
}
void driver::parse() {
    reset_before_parse_process();
    do {
	if(more_context_required_) {
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
    more_context_required_ = false;
    location_.reset_to_query_start();
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
