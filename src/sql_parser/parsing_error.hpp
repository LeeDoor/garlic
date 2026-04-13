#pragma once
#include "cell_type.hpp"
#include "error_stage.hpp"
#include "position.hpp"

namespace garlic::sql_parser {

/// Defines an error while parsing process.
struct ParsingError {
    bool more_context_required; 
    ErrorStage stage; 
    Position location;
    StringType message;
};

}
