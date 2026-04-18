#pragma once

namespace garlic {

using CharType = char;
using StringType = std::basic_string<CharType>;
using StringViewType = std::basic_string_view<CharType>;
using IntType = int;
using FloatType = float;
template<typename T>
using ArrayType = std::vector<T>;

template<typename T>
concept IsStoringColumnType = 
    std::is_same_v<T, StringType>
    || std::is_same_v<T, IntType> 
    || std::is_same_v<T, FloatType>;

template<typename T>
concept IsReadonlyColumnType = 
    std::is_same_v<T, StringViewType>
    || std::is_same_v<T, IntType> 
    || std::is_same_v<T, FloatType>;

template<typename T>
concept IsAnyColumnType = IsStoringColumnType<T> || IsReadonlyColumnType<T>;

enum CellType {
    String, Int, Float, Boolean, Error
};

}
