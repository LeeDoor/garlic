#pragma once
#include "cell_value.hpp"

namespace garlic::abstract_syntax_tree {

using namespace garlic::table;

using ExpectedCellValue = ExpectedOrStr<sptr<CellValue>>;

}
