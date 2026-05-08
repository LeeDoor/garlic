#include "table_value_expression.hpp"
#include "cell_value_gatherer.hpp"

namespace garlic {

ExpectedCellValue TableValueExpression::resolve(const TablesGathered& gatherers) const {
    if(!gatherers.contains(table_name_)) {
	return std::unexpected("Missed table name " + table_name_ + " in FROM section.");
    }
    auto gatherer = gatherers.at(table_name_);
    return gatherer->get_table_value(column_name_);
}

TableValueExpression::UsedTables TableValueExpression::get_used_tables() const {
    return { table_name_ };
}

}
