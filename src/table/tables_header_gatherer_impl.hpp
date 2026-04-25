#pragma once
#include "cell_type.hpp"
#include "expected_column_type.hpp"

namespace garlic {

template<typename T>
concept TablesHeaderGathererImpl = requires (const T& tables_gatherer) {
    { tables_gatherer.get_tables_column_type(TableNameType{}, ColumnNameType{}) } -> std::convertible_to<ExpectedColumnType>;
};

}
