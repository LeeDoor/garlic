#pragma once
#include "cell_type.hpp"

namespace garlic::table {
class CellValue;
}

namespace garlic::abstract_syntax_tree {

class CellValueGatherer {
  public:
	virtual ~CellValueGatherer() = default;

	virtual bool is_table_empty() const = 0;
	/// Gets value for given column_name. To change pointer, use @ref jump_to_next_row .
	virtual sptr<table::CellValue> get_table_value(const table::ColumnNameType &column_name) = 0;
	/// Moves row pointer to next row. If overflowed, pointer is reset to 0 and true returned.
	virtual bool jump_to_next_row() = 0;
};

} // namespace garlic::abstract_syntax_tree
