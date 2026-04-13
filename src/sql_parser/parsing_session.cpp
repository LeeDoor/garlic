#include "parsing_session.hpp"
#include "manual_io.hpp"

namespace garlic::sql_parser {

std::tuple<ParsingSession::ParsingResults, ParsingSession::ContinuationState> ParsingSession::get_parsing_result() {
    if(!parsing_results_.empty()) {
	auto& last = parsing_results_.back();
	if(last.is_error() && last.as_error().more_context_required) {
	    location.reset_to_query_start();
	} else {
	    finished_previous_query();
	}
    }
    return {
	std::move(parsing_results_),
	ContinuationState{ location, waiting_query_content }
    };
}

void ParsingSession::invoke_error(ErrorStage stage, const std::string& msg) {
    if(recovery) return;
    recovery = !more_context_required;
    auto& loc = location;
    auto error_relative_location = location.token_start();
    if(is_manual_IO())
	error_relative_location -= std::min(loc.content_query_start(), loc.line_start());
    parsing_results_.push_back(
    ParsingResult{
	ParsingError{ 
	    .more_context_required = more_context_required,
	    .stage = stage, 
	    .location = error_relative_location,
	    .message = msg
	}, current_position()
    });
}
void ParsingSession::query_parsed(uptr<Query> query) {
    parsing_results_.push_back({ std::move(query), current_position() });
    finished_previous_query();
}
void ParsingSession::blank_parsed() {
    auto current_chars_read = current_position();
    if(parsing_results_.empty() || !parsing_results_.back().is_blank()) {
	parsing_results_.push_back(ParsingResult{ current_chars_read });
    } else {
	parsing_results_.back().update_end_pos(current_chars_read);
    }
    finished_previous_query();
}
void ParsingSession::error_parsed() {
    if(parsing_results_.empty() || !parsing_results_.back().is_error()) {
	throw std::logic_error("ErrorParsed careset();lled but last parsing result is not an error");
    }
    recovery = false;
    auto& last = parsing_results_.back();
    last.update_end_pos(current_position());
    if(!last.as_error().more_context_required)
	finished_previous_query();
}

void ParsingSession::finished_previous_query() {
    location.on_raw_query_start();
    waiting_query_content = true;
}

void ParsingSession::met_eof() {
    more_context_required = true;
}

Position ParsingSession::current_position() const {
    return location.cur();
}

}
