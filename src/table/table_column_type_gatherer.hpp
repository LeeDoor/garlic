#pragma once
#include "cell_type.hpp"
#include "column_info.hpp"
#include "expected_column_type.hpp"

namespace garlic {

using TableHeader = std::vector<ColumnInfo>;

template<typename T>
concept TableColumnTypeGatherer = requires (const T& table_header_gatherer) {
    { table_header_gatherer.get_column_type(ColumnNameType{}) } -> std::convertible_to<ExpectedColumnType>;
    { table_header_gatherer.get_header() } -> std::convertible_to<TableHeader>;
};

}
