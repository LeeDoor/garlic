#pragma once
#include "cell_value.hpp"

namespace garlic::abstract_syntax_tree {

using ExpectedCellValue = ExpectedOrStr<sptr<CellValue>>;

}
