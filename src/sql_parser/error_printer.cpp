#include "error_printer.hpp"
#include "manual_io.hpp"

namespace garlic::sql_parser {

const std::unordered_map<ErrorStage, std::string>ErrorPrinter::stage_str = {
    { Lexing, "LEXICAL_ERROR" },
    { Parsing, "SYNTAX_ERROR" },
    { SemanticAnalysis, "SEMANTIC_ERROR" },
    { Runtime, "RUNTIME_ERROR" },
};

ErrorPrinter::ErrorPrinter(std::ostream& os) : os_{ os } {}
ErrorPrinter::ErrorPrinter() : ErrorPrinter(std::cerr) {}

void ErrorPrinter::print_error(const ParsingError& error) const {
    print_error_code(error.stage);
	os_ << "at [" << error.location << "] "
	<< error.message << std::endl << std::endl;
}
void ErrorPrinter::print_error(const RuntimeError& error) const {
    print_error_code(Runtime);
	os_ << error << std::endl << std::endl;
}

void ErrorPrinter::print_error_code(ErrorStage stage) const {
    os_ << error_highlight_bash_color() 
        << "[" << stage_str.at(stage) << "]" 
        << reset_bash_color() << " ";
}

}
