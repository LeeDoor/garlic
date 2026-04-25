#pragma once
#include "database.hpp"

namespace garlic {

using TablesHeaderGatherer = Database;
static_assert(TablesHeaderGathererImpl<TablesHeaderGatherer>);

}
