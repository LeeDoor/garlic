#pragma once
#include "cell_type.hpp"
#include "cell_int_value.hpp"

namespace garlic {

class CellIntValue; 
class CellFloatValue; 
class CellStringValue;

template<IsAnyColumnType T>
struct get_cell_type;

template<>
struct get_cell_type<StringViewType> {
    using Type = CellStringValue;
};

template<>
struct get_cell_type<StringType> {
    using Type = CellStringValue;
};

template<>
struct get_cell_type<FloatType> {
    using Type = CellFloatValue;
};

template<>
struct get_cell_type<IntType> {
    using Type = CellIntValue;
};

}
