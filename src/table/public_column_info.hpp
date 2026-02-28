#pragma once
#include "column_info.hpp"

namespace garlic {

struct PublicColumnInfo {
    CellType type;
    ColumnNameType name;
    size_t size_characters;
};

}
