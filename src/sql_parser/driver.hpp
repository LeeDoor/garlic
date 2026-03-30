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

    void parse();
    bool parse_repl();

    void scan_end();
    void scan_begin();
    void invoke_error(ErrorStage stage, const std::string& err);

    bool more_context_required();
    void met_eof();
    void query_executed();

private:
    static inline bool is_manual_IO();
    static void print_prompt();
    void reset_before_parse_process();
    void reset_before_parsing_iteration();
    void read_input_to_query();
    void log_error(ErrorStage stage, const std::string& err) const;
    void shrink_executed_queries();

    bool debug_mode_ {};
    yy::location location_ {};
    bool is_eof_ {};
    bool more_context_required_ {};
    bool more_context_available_ {};
    std::string query_ {};
    std::string input_line_ {};
    size_t executed_queries_ {};
};

}
