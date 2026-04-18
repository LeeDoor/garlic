#pragma once
#include "cell_type.hpp"
#include "cell_value.hpp"

namespace garlic {

using UnexpectedCellValue = StringType;
using ExpectedCellValue = std::expected<sptr<CellValue>, UnexpectedCellValue>;

}
