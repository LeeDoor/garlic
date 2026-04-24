#pragma once
#include "cell_type.hpp"

namespace garlic {

template<typename T>
concept TableHeaderGatherer = requires (const T& table_header_gatherer) {
    { table_header_gatherer.get_column_type(ColumnNameType{}) } -> std::convertible_to<CellType>;
};

}
