#pragma once
#include "database.hpp"

namespace garlic::abstract_syntax_tree {

using namespace garlic::table;

using TableValueGathererFactory = Database;
static_assert(CellValueGathererFactoryImpl<TableValueGathererFactory>);

} // namespace garlic::abstract_syntax_tree
