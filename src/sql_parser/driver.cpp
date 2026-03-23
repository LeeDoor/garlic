#include "driver.hpp"
#include "parser.tab.hpp"

namespace garlic::sql_parser {

driver::driver(bool debug_mode)
: debug_mode_{ debug_mode }
{}

int driver::parse () {
    yy::parser parse (*this);
    parse.set_debug_level (debug_mode_);
    scan_begin();
    int res = -1;
    res = parse ();
    scan_end ();
    return res;
}
void driver::log_error(ErrorStage stage, const std::string& err) const {
    static const std::unordered_map<ErrorStage, std::string> stage_str {
	{ Lexing, "LEXICAL_ERROR" },
	{ Parsing, "SYNTAX_ERROR" },
	{ SemanticAnalysis, "SEMANTIC_ERROR" },
    };
    std::cerr 
	<< "[" << stage_str.at(stage) << "] "
	<< " at [" << location_ << "]: "
	<< err << std::endl;
}

}
