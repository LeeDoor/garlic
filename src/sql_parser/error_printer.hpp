#pragma once
#include "error_stage.hpp"
#include "parsing_error.hpp"

namespace garlic::sql_parser {

/// ErrorPrinter prints @ref ParsingError to given stream.
class ErrorPrinter {
public:
    explicit ErrorPrinter(std::ostream& os);
    ErrorPrinter();

    void print_error(const ParsingError& error) const;

private:
    static const std::unordered_map<ErrorStage, std::string> stage_str; 
    std::ostream& os_;
};

}
