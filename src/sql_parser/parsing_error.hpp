#pragma once
#include "error_stage.hpp"
#include "location.hh"

namespace garlic::sql_parser {

struct ParsingError {
    ErrorStage stage;
    yy::location location;
    std::string message;
};

}
