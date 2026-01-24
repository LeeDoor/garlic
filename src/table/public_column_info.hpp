#pragma once
#include "cell_type.hpp"

namespace garlic {

struct PublicColumnInfo {
    CellType type;
    std::string name;
    size_t size_characters;
};

}
