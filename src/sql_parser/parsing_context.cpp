#include "parsing_context.hpp"
#include "manual_io.hpp"

namespace garlic::sql_parser {

ParsingContext::ParsingContext(bool debug)
: debug_mode_{ debug }
{}

void ParsingContext::reset_context() {
    context_ = {};
}

decltype(auto) ParsingContext::create_parser(StringViewType query_string) {
    auto deleter = [this](yy::parser* p) {
	scan_end();
	delete p;
    };
    auto parser = std::unique_ptr<yy::parser, decltype(deleter)>(new yy::parser(*this), std::move(deleter));
    parser->set_debug_level(debug_mode_);
    scan_begin(query_string);
    return parser;
}

ParsingContext::ParsingResults& ParsingContext::parse(StringViewType query_string) & {
    reset_before_parse();
    auto parser = create_parser(query_string);
    (*parser)();
    return parsing_results_;
}

void ParsingContext::reset_before_parse() {
    if(!parsing_results_.empty()) {
	auto& last = parsing_results_.back();
	if(last.is_error() && last.as_error().more_context_required) {
	    context_.location.reset_to_query_start();
	}
    } 
    context_.multiline_string_buffer = "";
    context_.left_ok = 1;
    context_.recovery = false;
    more_context_required_ = false;
    parsing_results_.clear();
}

void ParsingContext::invoke_error(ErrorStage stage, const std::string& msg) {
    if(context_.recovery) return;
    context_.recovery = !more_context_required_;
    parsing_results_.push_back(
    ParsingResult{
	ParsingError{ 
	    .more_context_required = more_context_required_,
	    .stage = stage, 
	    .location = context_.location.token_start(), 
	    .message = std::move(msg) 
	}, now_at_char()
    });
}
void ParsingContext::query_parsed(uptr<Query> query) {
    parsing_results_.push_back({ std::move(query), now_at_char() });
    location_to_query_start();
}
void ParsingContext::blank_parsed() {
    auto current_chars_read = now_at_char();
    if(parsing_results_.empty() || !parsing_results_.back().is_blank()) {
	parsing_results_.push_back(ParsingResult{ current_chars_read });
    } else {
	parsing_results_.back().update_end_idx(current_chars_read);
    }
    location_to_query_start();
}
void ParsingContext::error_parsed() {
    if(parsing_results_.empty() || !parsing_results_.back().is_error()) {
	throw std::logic_error("ErrorParsed Called but last parsing result is not an error");
    }
    parsing_results_.back().update_end_idx(now_at_char());
    location_to_query_start();
}
void ParsingContext::location_to_query_start() {
    auto& location = context_.location;
     if (is_manual_IO()) {
        location.reset();
     } else {
        location.on_query_start();
     }
}

void ParsingContext::met_eof() {
    more_context_required_ = true;
}

size_t ParsingContext::now_at_char() const {
    return context_.location.cur().chars_amount();
}

}
