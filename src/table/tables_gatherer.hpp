#pragma once
#include "cell_type.hpp"

namespace garlic {

using UnexpectedColumnType = StringType;
using ExpectedColumnType = std::expected<CellType, UnexpectedColumnType>;

template<typename T>
concept TablesGatherer = requires (const T& tables_gatherer) {
    { tables_gatherer.get_tables_column_type(TableNameType{}, ColumnNameType{}) } -> std::convertible_to<ExpectedColumnType>;
};

}
