#pragma once

namespace garlic::sql_parser {

/// Defines the stage of occured error.
enum ErrorStage { Lexing, Parsing, SemanticAnalysis, Runtime };

}
