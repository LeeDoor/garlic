#pragma once

namespace garlic {

using CharType = char;
using StringType = std::basic_string<CharType>;
using StringViewType = std::basic_string_view<CharType>;
using ColumnNameType = StringType;
using TableNameType = StringType;
using IntType = int;
using FloatType = float;
template<typename T>
using ArrayType = std::vector<T>;

template<typename... Ts>
struct TypeList {};

using ReadonlyColumnTypes = TypeList<
    StringViewType,
    IntType,
    FloatType,
    bool
>;

using StoringColumnTypes = TypeList<
    StringType,
    IntType,
    FloatType,
    bool
>;

template<typename VerifiedType, typename TypeContainer>
struct MatchesTypeArray;

template<typename VerifiedType, typename... TypeContainer>
struct MatchesTypeArray<VerifiedType, TypeList<TypeContainer...>> : 
    std::bool_constant<(std::is_same_v<VerifiedType, TypeContainer> || ...)> {};

template<typename T>
concept IsStoringColumnType = MatchesTypeArray<T, garlic::StoringColumnTypes>::value;

template<typename T>
concept IsReadonlyColumnType = MatchesTypeArray<T, garlic::ReadonlyColumnTypes>::value;

template<typename T>
concept IsAnyColumnType = IsStoringColumnType<T> || IsReadonlyColumnType<T>;

enum CellType {
    String, Int, Float, Boolean, Error
};

}
