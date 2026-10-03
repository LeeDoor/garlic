#pragma once
#include "cell_type.hpp"

namespace garlic::table {
class CellValue;
}

namespace garlic::abstract_syntax_tree {

using namespace garlic::table;

class CellValueGatherer;

using TablesGathered = std::unordered_map<TableNameType, sptr<CellValueGatherer>>;
using SingleTableRow = std::unordered_map<table::ColumnNameType, sptr<CellValue>>;
using MultipleTablesRow = std::unordered_map<TableNameType, SingleTableRow>;

} // namespace garlic::abstract_syntax_tree
