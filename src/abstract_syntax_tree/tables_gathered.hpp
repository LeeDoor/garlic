#pragma once
#include "cell_type.hpp"

namespace garlic::abstract_syntax_tree {

using namespace garlic::table;

class CellValueGatherer;

using TablesGathered = std::map<TableNameType, sptr<CellValueGatherer>>;

} // namespace garlic::abstract_syntax_tree
