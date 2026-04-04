#pragma once
#include "cell_type.hpp"
#include "error_stage.hpp"
#include "location.hh"

namespace garlic::sql_parser {

class ErrorPrinter {
    using LocationType = yy::location;
public:
    explicit ErrorPrinter(std::ostream& os) : os_{ os } {}
    ErrorPrinter() : ErrorPrinter(std::cerr) {}

    void print_error(ErrorStage stage, LocationType location, const StringType& msg) {
	os_ << "[" << stage_str.at(stage) << "] "
	    << "at [" << location << "] "
	    << msg << std::endl;
    }

private:
    static const std::unordered_map<ErrorStage, std::string> stage_str; 
    std::ostream& os_;
};

}
