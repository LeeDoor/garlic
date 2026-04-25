#include "parsing_session.hpp"
#include "manual_io.hpp"
#include "tables_header_gatherer_impl.hpp"

namespace garlic::sql_parser {

ParsingSession::IntermediateResult ParsingSession::get_parsing_result() {
    if(!parsing_results_.empty()) {
	auto& last = parsing_results_.back();
	if(last.is_error() && last.as_error().more_context_required) {
	    location_.reset_to_query_content_start();
	} else {
	    finished_previous_query();
	}
    }
    return {
	std::move(parsing_results_),
	ContinuationState{ location_, waiting_query_content_ },
	(location_.cur() - location_.initial()).get_characters()
    };
}

void ParsingSession::invoke_error(ErrorStage stage, const std::string& msg) {
    if(recovery_) return;
    recovery_ = !more_context_required_;
    auto& loc = location_;
    auto error_relative_location = location_.token_start();
    if(is_manual_IO())
	error_relative_location -= std::min(loc.content_query_start(), loc.line_start());
    parsing_results_.push_back(
    ParsingResult{
	ParsingError{ 
	    .more_context_required = more_context_required_,
	    .stage = stage, 
	    .location = error_relative_location,
	    .message = msg
	}
    });
}
void ParsingSession::query_parsed(uptr<Query> query) {
    parsing_results_.push_back({ std::move(query) });
    finished_previous_query();
}
void ParsingSession::blank_parsed() {
    finished_previous_query();
}
void ParsingSession::error_parsed() {
    if(parsing_results_.empty() || !parsing_results_.back().is_error()) {
	throw std::logic_error("ErrorParsed careset();lled but last parsing result is not an error");
    }
    recovery_ = false;
    auto& last = parsing_results_.back();
    if(!last.as_error().more_context_required)
	finished_previous_query();
}

void ParsingSession::finished_previous_query() {
    waiting_query_content_ = true;
}

void ParsingSession::on_each_token() {
    location_.on_token_start();
    --left_ok_;
}
void ParsingSession::met_newline(size_t token_len) {
    location_.cur().lines(token_len);
    location_.on_line_start();

}
void ParsingSession::met_content(size_t token_len) {
    if(waiting_query_content_) { 
	waiting_query_content_ = false; 
	location_.on_content_query_start(); 
    } 
    location_.cur().columns(token_len); 

}
void ParsingSession::met_space(size_t token_len) {
    location_.cur().columns(token_len); 
}
void ParsingSession::met_word_delimeter() {
    { left_ok_ = 1; }

}
yy::parser::symbol_type ParsingSession::lexing_error(const StringType& msg) {
    invoke_error(ErrorStage::Lexing, msg); 
    return yy::parser::make_YYerror(location_.cur()); 
}
std::optional<yy::parser::symbol_type> ParsingSession::whitespace_separated(const StringType& token_name) {
    if(left_ok_ < 0) { 
	return lexing_error("Token " + token_name + " should be whitespace-separated"); 
    }
    return std::nullopt;
}

void ParsingSession::append_line(const StringType& str) { 
    multiline_string_buffer_ += str; 
}
StringType ParsingSession::get_multiline_string() { 
    return std::move(multiline_string_buffer_); 
}
void ParsingSession::met_eof() {
    more_context_required_ = true;
}

Position ParsingSession::current_position() const {
    return location_.cur();
}

TablesHeaderGatherer ParsingSession::get_database() const {
    return TablesHeaderGatherer{ {} };
}

}
