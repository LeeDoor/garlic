#pragma once
#include "cell_type.hpp"

namespace garlic {

class ConditionResolver {
public:
    virtual ~ConditionResolver() = default;

    virtual StringViewType get_string_value(size_t column) const = 0;
    virtual IntType get_int_value(size_t column) const = 0;
    virtual FloatType get_float_value(size_t column) const = 0;
    virtual void set_row_id(size_t new_row_id) = 0;
};

}
