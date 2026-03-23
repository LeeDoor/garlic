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

    int parse();
    void scan_end();
    void scan_begin();
    void log_error(ErrorStage stage, const std::string& err) const;

private:
    yy::location location_;
    bool debug_mode_;
};

}
