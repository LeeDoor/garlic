#pragma once
#include "cell_type.hpp"

namespace garlic {

class CellValueGatherer;

using TablesGathered = std::map<TableNameType, sptr<CellValueGatherer>>;

}
