#pragma once
#include "cell_value_gatherer.hpp"

namespace garlic::sql_parser {

class DumbCellValueGatherer : public CellValueGatherer {
public:
    sptr<CellValue> get_table_value(const std::string& ) override {
	throw std::logic_error("Not supposed to be invoked; stub class");
    }
};

}
