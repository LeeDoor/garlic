#pragma once
#include "cell_type.hpp"

namespace garlic {

struct ColumnInfo {
    CellType type;
    std::string name;
    size_t size_bytes;
    size_t offset;
};

}
