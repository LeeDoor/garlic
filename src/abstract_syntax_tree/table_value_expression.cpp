#include "table_value_expression.hpp"

namespace garlic {

ExpectedCellValue TableValueExpression::resolve(sptr<CellValueGatherer> gatherer) const {
    return gatherer->get_table_value(column_name_);
}

}
