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
    void log_error(ErrorStage stage, const std::string& err) const;

    /// Called if lexing process requires more user input to continue.
    bool more_context_required();

    void met_eof();
    bool is_eof() const;

private:
    static inline bool should_print_prompt();

    yy::location location_;
    bool debug_mode_;
    bool is_eof_ = false;
    bool more_context_required_ = false;
    bool more_context_available_ = true;
    std::string query_;
    std::string input_line_;
};

}
