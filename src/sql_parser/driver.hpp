#pragma once
#include "parser.tab.hpp"

#define YY_DECL \
    yy::parser::symbol_type yylex (driver& drv)

YY_DECL;

namespace garlic::sql_parser {

class driver
{
public:
    explicit driver(bool debug_mode);

    yy::location& location() { return location_; }

    int parse();
    void scan_end();
    void scan_begin();

private:
    yy::location location_;
    bool debug_mode_;
};

}
