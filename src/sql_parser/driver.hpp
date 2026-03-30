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
    /*! @returns true if more input required and false overwise. */
    bool parse_repl();

    void scan_end();
    void scan_begin();
    void invoke_error(ErrorStage stage, const std::string& err);

    /// Called if lexing process requires more user input to continue.
    bool more_context_required();

    void met_eof();
    bool is_eof() const;

    void shrink_last_query();

private:
    static inline bool is_manual_IO();
    static void print_prompt();
    void reset_before_parse();
    void reset_before_iteration();
    void read_input_to_query();
    void log_error(ErrorStage stage, const std::string& err) const;

    bool debug_mode_ {};
    yy::location location_ {};
    bool is_eof_ {};
    bool more_context_required_ {};
    bool more_context_available_ {};
    std::string query_ {};
    std::string input_line_ {};
};

}
