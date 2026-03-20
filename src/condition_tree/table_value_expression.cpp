#include "table_value_expression.hpp"

namespace garlic {

TableValueExpression::TableValueExpression(std::string column_name)
: column_name_{ column_name }
{} 

CellValue::CellValuePtr TableValueExpression::get_value(TableValueGathererPtr gatherer) const {
    return gatherer->get_table_value(column_name_);
}

}
