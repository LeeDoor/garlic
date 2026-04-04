#include "error_printer.hpp"

namespace garlic::sql_parser {

const std::unordered_map<ErrorStage, std::string>ErrorPrinter::stage_str = {
    { Lexing, "LEXICAL_ERROR" },
    { Parsing, "SYNTAX_ERROR" },
    { SemanticAnalysis, "SEMANTIC_ERROR" },
};


}
