#include "table_value_expression.hpp"

namespace garlic {

TableValueExpression::TableValueExpression(std::string column_name)
: column_name_{ column_name }
{} 

sptr<CellValue> TableValueExpression::get_value(sptr<TableValueGatherer> gatherer) const {
    return gatherer->get_table_value(column_name_);
}

}
