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
	} else {
	    finished_previous_query();
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
    auto& loc = context_.location;
    auto error_relative_location = context_.location.token_start();
    if(is_manual_IO())
	error_relative_location -= std::min(loc.content_query_start(), loc.line_start());
    parsing_results_.push_back(
    ParsingResult{
	ParsingError{ 
	    .more_context_required = more_context_required_,
	    .stage = stage, 
	    .location = error_relative_location,
	    .message = msg
	}, current_position()
    });
}
void ParsingContext::query_parsed(uptr<Query> query) {
    parsing_results_.push_back({ std::move(query), current_position() });
    finished_previous_query();
}
void ParsingContext::blank_parsed() {
    auto current_chars_read = current_position();
    if(parsing_results_.empty() || !parsing_results_.back().is_blank()) {
	parsing_results_.push_back(ParsingResult{ current_chars_read });
    } else {
	parsing_results_.back().update_end_pos(current_chars_read);
    }
    finished_previous_query();
}
void ParsingContext::error_parsed() {
    if(parsing_results_.empty() || !parsing_results_.back().is_error()) {
	throw std::logic_error("ErrorParsed careset();lled but last parsing result is not an error");
    }
    context_.recovery = false;
    auto& last = parsing_results_.back();
    last.update_end_pos(current_position());
    if(!last.as_error().more_context_required)
	finished_previous_query();
}
void ParsingContext::finished_previous_query() {
    context_.location.on_raw_query_start();
    context_.waiting_query_content = true;
}

void ParsingContext::met_eof() {
    more_context_required_ = true;
}

Position ParsingContext::current_position() const {
    return context_.location.cur();
}

}
