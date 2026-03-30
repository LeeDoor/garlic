#include "driver.hpp"
#include "parser.tab.hpp"

namespace garlic::sql_parser {

driver::driver(bool debug_mode)
: debug_mode_{ debug_mode }
{ }

void driver::reset_before_parse() {
    query_.clear();
    input_line_.clear();
    more_context_available_ = true;
}
void driver::reset_before_iteration() {
    more_context_required_ = false;
    is_eof_ = false;
    if(is_manual_IO() && !more_context_required_) {
	location_.initialize();
    }
}
void driver::parse() {
    reset_before_parse();
    do {
	reset_before_iteration();
	print_prompt();
	read_input_to_query();
	parse_repl();
    } while (more_context_available_);
}
void driver::print_prompt() {
    if(is_manual_IO())
	std::cout << "#> " << std::flush;
}
void driver::read_input_to_query() {
    std::getline(std::cin, input_line_);
    more_context_available_ = !std::cin.eof(); 
    query_ += input_line_;
    query_ += "\n";
}
bool driver::parse_repl() {
    yy::parser parse(*this);
    parse.set_debug_level(debug_mode_);
    scan_begin();
    parse();
    scan_end();
    return more_context_required_;
}
void driver::invoke_error(ErrorStage stage, const std::string& err) {
    if(!(is_eof() && more_context_required())) {
	log_error(stage, err);
	shrink_last_query();
    }
}
void driver::log_error(ErrorStage stage, const std::string& err) const {
    static const std::unordered_map<ErrorStage, std::string> stage_str {
	{ Lexing, "LEXICAL_ERROR" },
	{ Parsing, "SYNTAX_ERROR" },
	{ SemanticAnalysis, "SEMANTIC_ERROR" },
    };
    std::cerr 
	<< "[" << stage_str.at(stage) << "] "
	<< "at [" << location_ << "] "
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
void driver::shrink_last_query() {
    auto first_semicolon = query_.find(';');
    if(first_semicolon == query_.npos)
	query_.clear();
    query_.erase(0, first_semicolon + 1);
}

#ifdef _WIN32
inline bool driver::should_print_prompt() {
    return _isatty(_fileno(stdin)) && _isatty(_fileno(stdout));
}
#else
inline bool driver::is_manual_IO() {
    return isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
}
#endif

}
