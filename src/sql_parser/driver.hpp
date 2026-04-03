#pragma once
#include "parser.tab.hpp"

#define YY_DECL \
    yy::parser::symbol_type yylex (driver& drv)

YY_DECL;

namespace garlic::sql_parser {

class driver {
public:
    enum ErrorStage { Lexing, Parsing, SemanticAnalysis };

    explicit driver(bool debug_mode = false);

    yy::location& location() & { return location_; }
    void invoke_error(ErrorStage stage, const std::string& err);
    void query_executed();
    void met_eof();
    void parse();
    void memorize_token_begin_loc();

private:
    void log_error(ErrorStage stage, const std::string& err) const;

    void reset_before_parse_process();
    static void print_prompt();
    void read_input_to_query();
    void reset_before_parsing_iteration();

    void parse_repl();
    void scan_begin();
    void scan_end();

    void shrink_executed_queries();
    static inline bool is_manual_IO();

    bool debug_mode_ {};
    yy::location location_ {};
    yy::location token_begin_location_ {};
    yy::location current_query_beginning_ {};
    bool is_eof_ {};
    bool more_context_available_ {};
    std::string query_ {};
    std::string input_line_ {};
    size_t executed_queries_ {};
};

}
