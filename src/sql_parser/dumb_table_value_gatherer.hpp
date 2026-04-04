#pragma once
#include "driver.hpp"

namespace garlic::sql_parser {

class DumbTableValueGatherer : public TableValueGatherer {
public:
    sptr<CellValue> get_table_value(const std::string& ) override {
	throw std::logic_error("Not supposed to be invoked; stub class");
    }
};

}
