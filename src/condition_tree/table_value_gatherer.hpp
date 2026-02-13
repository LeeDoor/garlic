#pragma once
#include "cell_value.hpp"

namespace garlic {

class TableValueGatherer {
protected:
    using CellValuePtr = std::shared_ptr<CellValue>;
public:
    virtual ~TableValueGatherer() = default;

    virtual CellValuePtr get_table_value(const std::string& column_name) = 0;
};

}
