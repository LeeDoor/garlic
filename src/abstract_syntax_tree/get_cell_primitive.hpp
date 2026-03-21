#pragma once
#include "cell_type.hpp"

namespace garlic {

class CellValue;
class CellIntValue;
class CellFloatValue;
class CellStringViewValue;

template<typename T>
struct get_cell_primitive;

template<>
struct get_cell_primitive<CellIntValue> {
    using Type = IntType;
};
template<>
struct get_cell_primitive<CellFloatValue> {
    using Type = FloatType;
};
template<>
struct get_cell_primitive<CellStringViewValue> {
    using Type = StringViewType;
};

}
