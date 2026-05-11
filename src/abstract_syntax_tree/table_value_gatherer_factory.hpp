#pragma once
#include "database.hpp"

namespace garlic {

using TableValueGathererFactory = Database;
static_assert(CellValueGathererFactoryImpl<TableValueGathererFactory>);

}
