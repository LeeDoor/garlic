#pragma once
#include "cell_type.hpp"
#include "error_stage.hpp"
#include "location.hh"
#include "parsing_error.hpp"

namespace garlic::sql_parser {

class ErrorPrinter {
    using LocationType = yy::location;
public:
    explicit ErrorPrinter(std::ostream& os) : os_{ os } {}
    ErrorPrinter() : ErrorPrinter(std::cerr) {}

    void print_error(const ParsingError& error) const {
	os_ << "[" << stage_str.at(error.stage) << "] "
	    << "at [" << error.location << "] "
	    << error.message << std::endl;
    }

private:
    static const std::unordered_map<ErrorStage, std::string> stage_str; 
    std::ostream& os_;
};

}
