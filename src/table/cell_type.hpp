#pragma once

namespace garlic {

using CharType = char;
using StringType = std::basic_string<CharType>;
using StringViewType = const std::basic_string_view<CharType>;
using IntType = int;
using FloatType = float;

enum CellType {
    String, Int, Float
};
template<typename T>
concept IsColumnType = 
    std::is_same_v<T, StringType>
    || std::is_same_v<T, IntType> 
    || std::is_same_v<T, FloatType>;

template<typename T>
concept IsReadonlyColumnType = 
    std::is_same_v<T, StringViewType>
    || std::is_same_v<T, IntType> 
    || std::is_same_v<T, FloatType>;

template<CellType cell>
constexpr size_t get_type_size() {
    if constexpr (cell == CellType::Int) {
        return sizeof(IntType);
    } else if constexpr (cell == CellType::Float) {
        return sizeof(FloatType);
    } else {
        throw std::logic_error("Unknown type to get size of");
    }
}

constexpr size_t get_type_size(CellType cell) {
    switch(cell) {
        case CellType::Int: return get_type_size<CellType::Int>();
        case CellType::Float: return get_type_size<CellType::Float>();
        default: throw std::logic_error("Unknown type to get size of");
    }
}


}
