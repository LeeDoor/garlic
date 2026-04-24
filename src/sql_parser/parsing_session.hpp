#pragma once
#include "parser.tab.hpp"
#include "parsing_location.hpp"
#include "cell_type.hpp"
#include "error_stage.hpp"
#include "parsing_result.hpp"

#define YY_DECL \
    yy::parser::symbol_type yylex (ParsingSession& session)

YY_DECL;

namespace garlic { class Query; }
namespace garlic::sql_parser {

class ParserEngine;

/// Handles each parse() call.
class ParsingSession {
public:
    using ParsingResults = std::list<ParsingResult>;

    struct ContinuationState {
	ParsingLocation location;
	bool waiting_query_content;
    };

    ParsingSession() {}
    ParsingSession(ContinuationState cont_state)
    {
	location_ = ParsingLocation::initialize_from(cont_state.location);
	waiting_query_content_ = cont_state.waiting_query_content;
    }
    
    struct IntermediateResult {
	ParsingResults parsing_results;
	ContinuationState continuation_state;
	size_t characters_parsed;
    };

    IntermediateResult get_parsing_result();

    /// Called when valid non-empty query parsed.
    void query_parsed(uptr<Query> query);
    /// Called when valid query without content parsed.
    void blank_parsed();
    /// Called when reached a semicolon after an error.
    void error_parsed();
    /// Called on any error occured.
    void invoke_error(ErrorStage stage, const std::string& msg);

    /// called in YY_USER_ACTION, i.e. before every rule
    void on_each_token();
    /// call in every newline 
    /// character; used for location accounting
    void met_newline(size_t token_len);
    /// call in every meaningful token;
    /// used to track content query start location.
    void met_content(size_t token_len);
    /// Call in every space, t or similar;
    /// Used to modify location.
    void met_space(size_t token_len);
    /// Call in every word-separating whitespace;
    /// Used to distinguish words like SELECT<>AND<>OR
    void met_word_delimeter();
    /// Called on any EOF match
    void met_eof();

    /// call if token should be whitespace separated;
    /// i.e. to avoid "true ANDOR false".
    std::optional<yy::parser::symbol_type> whitespace_separated(const StringType& token_name);
    /// triggers lexical error with given message
    yy::parser::symbol_type lexing_error(const StringType& msg);

    /// Appends line to inner buffer to build multiline string.
    void append_line(const StringType& str);
    /// Returns moved string buffer.
    StringType get_multiline_string();
    /// returns current position as object.
    Position current_position() const;
    const ParsingLocation& location() const& { return location_; }

private:
    /// Called if just finished a query and started a new  one.
    void finished_previous_query();

    ParsingResults parsing_results_ {};
    ParsingLocation location_ {};
    StringType multiline_string_buffer_ {};
    bool recovery_ {};
    bool waiting_query_content_ {};
    bool more_context_required_ {};
    // set to 1 when seen a whitespace; 
    // decremented in YY_USER_ACTION; 
    // if == 0, it means the last character was a whitespace; 
    // if < 0, it wasn't
    int left_ok_ {1};
    const Database& database_;
};

}
