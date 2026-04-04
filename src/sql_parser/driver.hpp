#pragma once
#include "error_printer.hpp"
#include "parsing_location.hpp"
#include "query_io.hpp"
#include "parser.tab.hpp"
#include "sql_ast_executor.hpp"

#define YY_DECL \
    yy::parser::symbol_type yylex (driver& drv)

YY_DECL;

namespace garlic::sql_parser {

class driver {
public:
    explicit driver(bool debug_mode = false);

    ParsingLocation& location() & { return location_; }
    void invoke_error(ErrorStage stage, const std::string& msg);
    void query_executed();
    void query_executed(uptr<Query> query);
    void met_eof();
    void memorize_token_begin_loc();
    void parse();

private:
    void reset_before_parse_process();
    void reset_before_parsing_iteration();

    void parse_repl();
    void scan_begin();
    void scan_end();

    QueryIO query_io_ {};
    ErrorPrinter err_printer_ {};
    ParsingLocation location_ {};
    SqlAstExecutor ast_executor_ {};
    bool debug_mode_ {};
    bool more_context_required_ {};
    size_t executed_queries_ {};
};

}
