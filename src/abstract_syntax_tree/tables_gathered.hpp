#pragma once
#include "cell_type.hpp"

namespace garlic {

class CellValueGatherer;

using TablesGathered = std::unordered_map<TableNameType, sptr<CellValueGatherer>>;

}
