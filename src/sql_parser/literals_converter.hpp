#pragma once
#include "parsing_session.hpp"
#include "position.hpp"

namespace garlic::sql_parser {

yy::parser::symbol_type make_FLOAT(std::string_view s, const Position& curloc, ParsingSession& session);
yy::parser::symbol_type make_INTEGER(std::string_view s, const Position& curloc, ParsingSession& session);
yy::parser::symbol_type make_STRING(const std::string& s, const Position& curloc);
    
}
