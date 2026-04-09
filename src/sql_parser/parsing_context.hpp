#pragma once
#include "cell_type.hpp"
#include "error_stage.hpp"
#include "parsing_location.hpp"
#include "parser.tab.hpp"
#include "parsing_result.hpp"

#define YY_DECL \
    yy::parser::symbol_type yylex (ParsingContext& ctx)

YY_DECL;

namespace garlic::sql_parser {

class ParsingContext {
public:
    using ParsingResults = std::list<ParsingResult>;

    struct Context {
	ParsingLocation location {};
	std::string multiline_string_buffer {};
	int left_ok {1};
	bool recovery {};
    };

    explicit ParsingContext(bool debug = false);

    /// Driver API
    void reset_context();
    ParsingResults& parse(StringViewType query_string) &;

    /// Parser/Lexer API
    Context& context() & { return context_; }
    void invoke_error(ErrorStage stage, const std::string& msg);
    void query_parsed(uptr<Query> query);
    void blank_parsed();
    void error_parsed();
    void location_to_query_start();
    void met_eof();

private:
    void reset_before_parse();
    decltype(auto) create_parser(StringViewType query_string);
    size_t now_at_char() const;

    void scan_begin(StringViewType query_string);
    void scan_end();

    bool debug_mode_ {};
    bool more_context_required_ {};
    ParsingResults parsing_results_ {};
    Context context_ {};
};

}
