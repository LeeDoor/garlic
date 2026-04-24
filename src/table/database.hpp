#pragma once
#include "table_header_gatherer.hpp"
#include "tables_gatherer.hpp"
#include "typed_table.hpp"

namespace garlic {

/// Array of all tables in the database
template<typename TableHeaderGathererType = TypedTable>
    requires TableHeaderGatherer<TableHeaderGathererType>
    && TableValueGatherer<TableHeaderGathererType>
class Database {
private:
    using TablesContainer = std::unordered_map<TableNameType, sptr<TableHeaderGathererType>>;
public:
    Database(TablesContainer&& tables)
    : tables_{ std::move(tables) }
    {}

    ExpectedColumnType get_tables_column_type(const TableNameType& table_name, const ColumnNameType& column_name) const {
	if(!tables_.contains(table_name))
	    return std::unexpected("Table " + table_name + " does not exist.");
	sptr<TableHeaderGathererType> table = tables_.at(table_name);
	return table->get_column_header(column_name);
    }

private:
    TablesContainer tables_;
};

static_assert(TablesGatherer<Database<>>, "Database doesn't match the TablesGatherer concept");

}
