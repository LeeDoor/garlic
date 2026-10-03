#include "table_value_expression.hpp"

namespace garlic::abstract_syntax_tree {

ExpectedCellValue TableValueExpression::resolve(const MultipleTablesRow &row) const {
	if (!row.contains(table_name_)) {
        throw std::logic_error("Table " + table_name_ + " is missing in FROM clause. Called TableValueExpression::resolve without validation.");
	}
	auto tables_column = row.at(table_name_);
	if (!tables_column.contains(column_name_)) {
        throw std::logic_error("Column " + column_name_ + " is missing in Table " + table_name_ + ". ");
	}
	return tables_column.at(column_name_);
}

TableValueExpression::UsedTables TableValueExpression::get_used_tables() const { return {table_name_}; }

} // namespace garlic::abstract_syntax_tree
