#pragma once
#include "database.hpp"

namespace garlic::abstract_syntax_tree {

using TablesHeaderGatherer = Database;
static_assert(TablesHeaderGathererImpl<TablesHeaderGatherer>);

} // namespace garlic::abstract_syntax_tree
