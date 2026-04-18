#include "table_value_expression.hpp"

namespace garlic {

TableValueExpression::TableValueExpression(std::string column_name, CellType type)
: Expression{ type }
, column_name_{ column_name }
{} 

std::optional<StringType> TableValueExpression::validate() const { return std::nullopt; }

ExpectedCellValue TableValueExpression::get_value(sptr<TableValueGatherer> gatherer) const {
    return gatherer->get_table_value(column_name_);
}

}
