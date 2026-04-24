#include "error_printer.hpp"

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
    os_ << "[" << stage_str.at(error.stage) << "] "
	<< "at [" << error.location << "] "
	<< error.message << std::endl << std::endl;
}
void ErrorPrinter::print_error(const RuntimeError& error) const {
    os_ << "[" << stage_str.at(Runtime) << "] "
	<< error << std::endl << std::endl;
}

}
