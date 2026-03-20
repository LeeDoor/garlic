#pragma once
#include "cell_value.hpp"

namespace garlic {

class TableValueGatherer {
public:
    virtual ~TableValueGatherer() = default;

    virtual sptr<CellValue> get_table_value(const std::string& column_name) = 0;
};

}
