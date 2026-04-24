#pragma once
#include "cell_type.hpp"

namespace garlic {

struct TableColumnReference {
    TableNameType table_name;
    ColumnNameType column_name;
};

}
