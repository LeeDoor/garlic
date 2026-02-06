#pragma once
#include "cell_type.hpp"

namespace garlic {

class ConditionResolver {
public:
    virtual size_t get_cell_size(size_t column_number) = 0;
};

}
