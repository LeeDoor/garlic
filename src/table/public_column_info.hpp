#pragma once
#include "cell_type.hpp"

namespace garlic::table {

struct PublicColumnInfo {
	CellType type;
	StringType name;
	size_t size_characters;
};

} // namespace garlic::table
