#pragma once
#include "cell_type.hpp"
#include "error_stage.hpp"
#include "parsing_error.hpp"
#include "parsing_location.hpp"
#include "parser.tab.hpp"

#define YY_DECL \
    yy::parser::symbol_type yylex (ParsingContext& ctx)

YY_DECL;

namespace garlic::sql_parser {

class ParsingContext {
public:
    using Queries = std::list<uptr<Query>>;

    enum ParsingResult { Error, MoreContextRequired, Ok };

    explicit ParsingContext(bool debug = false);

    /// Driver API
    void reset();
    ParsingResult parse(StringViewType query_string);
    const std::optional<ParsingError>& get_error() const &;
    Queries& get_queries() &;
    size_t parsed_queries() const;

    /// Parser/Lexer API
    ParsingLocation& location() & { return location_; }
    void invoke_error(ErrorStage stage, const std::string& msg);
    void query_executed();
    void query_executed(uptr<Query> query);
    void met_eof();
    void memorize_token_begin_loc();

private:
    void reset_before_parse();
    decltype(auto) create_parser(StringViewType query_string);

    void scan_begin(StringViewType query_string);
    void scan_end();

    bool debug_mode_ {};
    bool more_context_required_ {};
    size_t parsed_queries_ {};
    Queries stored_queries_ {};
    std::optional<ParsingError> stored_error_ {};
    ParsingLocation location_ {};

};

}
