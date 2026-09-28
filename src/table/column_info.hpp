#pragma once
#include "cell_type.hpp"

namespace garlic::table {

struct ColumnInfo {
	CellType type;
	ColumnNameType name;
	size_t size_bytes;
	size_t offset;
};

} // namespace garlic::table
