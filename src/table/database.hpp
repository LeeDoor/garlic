#pragma once
#include "tables_header_gatherer.hpp"
#include "typed_table.hpp"

namespace garlic {

/// Array of all tables in the database
template<typename TableColumnTypeGathererT>
requires TableColumnTypeGatherer<TableColumnTypeGathererT>
class DatabaseImpl {
private:
    using TablesContainer = std::unordered_map<TableNameType, sptr<TableColumnTypeGathererT>>;
public:
    DatabaseImpl(TablesContainer&& tables)
    : tables_{ std::move(tables) }
    {}

    ExpectedColumnType get_tables_column_type(const TableNameType& table_name, const ColumnNameType& column_name) const {
	if(!tables_.contains(table_name))
	    return std::unexpected("Table " + table_name + " does not exist.");
	sptr<TableColumnTypeGathererT> table = tables_.at(table_name);
	return table->get_column_type(column_name);
    }

private:
    TablesContainer tables_;
};

using Database = DatabaseImpl<TypedTable>;

static_assert(TablesHeaderGatherer<Database>, "Database doesn't match the TablesGatherer concept");

}
