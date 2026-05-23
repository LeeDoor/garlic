#pragma once
#include "cell_type.hpp"
#include "expected_column_type.hpp"
#include "table_column_type_gatherer.hpp"

namespace garlic {

template<typename T>
concept TablesHeaderGathererImpl = requires (const T& tables_gatherer) {
    { tables_gatherer.get_tables_column_type(TableNameType{}, ColumnNameType{}) } -> std::convertible_to<ExpectedColumnType>;
    { tables_gatherer.get_tables_header(TableNameType{}) } -> std::convertible_to<ExpectedOrStr<TableHeader>>;
};

}
