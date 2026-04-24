#pragma once
#include "cell_type.hpp"

namespace garlic {
class CellValue;

class TableValueGatherer {
public:
    virtual ~TableValueGatherer() = default;

    virtual sptr<CellValue> get_table_value(const ColumnNameType& column_name) = 0;
};

}
