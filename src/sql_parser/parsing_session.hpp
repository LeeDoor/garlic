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
	location = cont_state.location;
	waiting_query_content = cont_state.waiting_query_content;
    }
    
    std::tuple<ParsingResults, ContinuationState> get_parsing_result();

    void query_parsed(uptr<Query> query);
    void blank_parsed();
    void error_parsed();
    void invoke_error(ErrorStage stage, const std::string& msg);

    void met_eof();
    void finished_previous_query();
    void met_eof(ParsingSession& session);
    Position current_position() const;

    ParsingLocation location {};
    StringType multiline_string_buffer {};
    int left_ok {1};
    bool recovery {};
    bool waiting_query_content {};
    bool more_context_required {};
 
private:
    ParsingContext& context_;
    ParsingResults parsing_results_ {};
};

}
