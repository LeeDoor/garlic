#pragma once
#include "cell_type.hpp"

namespace garlic {
class CellValue;

class CellValueGatherer {
public:
    virtual ~CellValueGatherer() = default;

    virtual bool is_table_empty() const = 0;
    virtual sptr<CellValue> get_table_value(const ColumnNameType& column_name) = 0;
    /// moves row pointer to next row. If overflowed, pointer is reset to 0 and true returned.
    virtual bool jump_to_next_row() = 0;
};

}
