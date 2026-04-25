#pragma once
#include "cell_type.hpp"

namespace garlic {

using UnexpectedColumnType = StringType;
using ExpectedColumnType = std::expected<CellType, UnexpectedColumnType>;

}
