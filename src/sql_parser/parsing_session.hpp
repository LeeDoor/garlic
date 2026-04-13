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

class ParsingContext;

class ParsingSession {
public:
    using ParsingResults = std::list<ParsingResult>;

    struct ContinuationState {
	ParsingLocation location;
	bool waiting_query_content;
    };

    ParsingSession(ParsingContext& ctx)
    : context_{ ctx }
    {}
    ParsingSession(ParsingContext& ctx, ContinuationState cont_state)
    : ParsingSession(ctx)
    {
	location_ = cont_state.location;
	waiting_query_content_ = cont_state.waiting_query_content;
    }
    
    std::tuple<ParsingResults, ContinuationState> get_parsing_result();

    const ParsingLocation& location() const& { return location_; }

    void query_parsed(uptr<Query> query);
    void blank_parsed();
    void error_parsed();
    void invoke_error(ErrorStage stage, const std::string& msg);

    void on_each_token();
    void met_newline(size_t token_len);
    void met_content(size_t token_len);
    void met_space(size_t token_len);
    void met_word_delimeter();
    void met_eof();

    std::optional<yy::parser::symbol_type> whitespace_separated(const StringType& token_name);
    yy::parser::symbol_type lexing_error(const StringType& msg);

    void append_line(const StringType& str);
    StringType get_multiline_string();
    void finished_previous_query();
    Position current_position() const;

private:
    ParsingContext& context_;
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
 
};

}
