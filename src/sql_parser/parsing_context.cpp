#include "parsing_context.hpp"
#include "manual_io.hpp"

namespace garlic::sql_parser {

ParsingContext::ParsingContext(bool debug)
: debug_mode_{ debug }
{}

void ParsingContext::reset_before_parse() {
    location_.reset_to_query_start();
    parsed_queries_ = 0;
    more_context_required_ = false;
    stored_error_ = std::nullopt;
}


ParsingContext::ParserPtr ParsingContext::create_parser() {
    return ParserPtr(new yy::parser(*this), std::move(deleter));
}

ParsingContext::ParsingResult ParsingContext::parse(StringViewType query_string) {
    reset_before_parse();
    yy::parser parse(*this);
    parse.set_debug_level(debug_mode_);
    scan_begin(query_string);
    parse();
    scan_end();
    if(stored_error_.has_value()) {
	if(more_context_required_) {
	    return MoreContextRequired;
	}
	return Error;
    }
    return Ok;
}

void ParsingContext::reset() {
    parsed_queries_ = 0;
    stored_queries_.clear();
    stored_error_ = std::nullopt;
    more_context_required_ = false;
    location_.reset();
}

const std::optional<ParsingError>& ParsingContext::get_error() const & {
    return stored_error_;
}
ParsingContext::Queries& ParsingContext::get_queries() & {
    return stored_queries_;
}
size_t ParsingContext::parsed_queries() const {
    return parsed_queries_;
}

void ParsingContext::invoke_error(ErrorStage stage, const std::string& msg) {
    stored_error_ = ParsingError{ stage, location_.token_start(), std::move(msg) };
    query_executed();
}
void ParsingContext::query_executed() {
    if(!more_context_required_)
	++parsed_queries_;
    if (is_manual_IO()) {
	location_.reset();
    } else {
	location_.on_query_start();
    }
}
void ParsingContext::query_executed(uptr<Query> query) {
    query_executed();
    stored_queries_.push_back(std::move(query));
}
void ParsingContext::met_eof() {
    more_context_required_ = true;
}
void ParsingContext::memorize_token_begin_loc() {
    location_.on_token_start();
}

}
