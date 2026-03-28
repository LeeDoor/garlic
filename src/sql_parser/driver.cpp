#include "driver.hpp"
#include "parser.tab.hpp"

namespace garlic::sql_parser {

driver::driver(bool debug_mode)
: debug_mode_{ debug_mode }
{}

int driver::parse() {
    while(true) {
	std::cout << "#> " << std::flush;
	more_context_available_ = static_cast<bool>(std::getline(std::cin, input_line_));
	query_ += input_line_;
	query_ += "\n";
	bool input_required = parse_repl();
	if(!input_required) {
	    query_.clear();   
	}
	if(!more_context_available_) break;
    }
    return 0;
}
bool driver::parse_repl() {
    yy::parser parse(*this);
    parse.set_debug_level(debug_mode_);
    scan_begin();
    parse();
    scan_end();
    return more_context_required_;
}
void driver::log_error(ErrorStage stage, const std::string& err) const {
    static const std::unordered_map<ErrorStage, std::string> stage_str {
	{ Lexing, "LEXICAL_ERROR" },
	{ Parsing, "SYNTAX_ERROR" },
	{ SemanticAnalysis, "SEMANTIC_ERROR" },
    };
    std::cerr 
	<< "[" << stage_str.at(stage) << "] "
	<< "at [" << location_ << "]: "
	<< err << std::endl;
}

bool driver::more_context_required() {
    more_context_required_ = true;
    is_eof_ = true;
    return more_context_available_;
}
void driver::met_eof() {
    is_eof_ = true;
}
bool driver::is_eof() const {
    return is_eof_;
}

}
