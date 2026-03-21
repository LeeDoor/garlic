#pragma once
#include "cell_type.hpp"

namespace garlic {

class CellIntValue; 
class CellFloatValue; 
class CellStringViewValue;

template<IsAnyColumnType T>
struct get_cell_type;

template<>
struct get_cell_type<StringViewType> {
    using Type = CellStringViewValue;
};

template<>
struct get_cell_type<StringType> {
    using Type = CellStringViewValue;
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
