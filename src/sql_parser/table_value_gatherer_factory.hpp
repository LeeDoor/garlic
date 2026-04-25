#pragma once
#include "database.hpp"

namespace garlic::sql_parser {

using TableValueGathererFactory = Database;
static_assert(TableValueGathererFactoryImpl<TableValueGathererFactory>);

}
