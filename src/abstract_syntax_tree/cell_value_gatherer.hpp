#pragma once
#include "cell_type.hpp"

namespace garlic {
class CellValue;

class CellValueGatherer {
public:
    virtual ~CellValueGatherer() = default;

    virtual bool is_table_empty() const = 0;
    /// Gets value for given column_name. To change pointer, use @ref jump_to_next_row .
    virtual sptr<CellValue> get_table_value(const ColumnNameType& column_name) = 0;
    /// Moves row pointer to next row. If overflowed, pointer is reset to 0 and true returned.
    virtual bool jump_to_next_row() = 0;
};

}
