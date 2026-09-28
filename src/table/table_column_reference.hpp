#pragma once
#include "cell_type.hpp"

namespace garlic::table {

struct TableColumnReference {
	TableNameType table_name;
	ColumnNameType column_name;
};

} // namespace garlic::table
