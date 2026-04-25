#include "select_query.hpp"
#include "table_query_result.hpp"

namespace garlic {

SelectQuery::SelectQuery()
{}
SelectQuery::SelectQuery(ColumnsContainer columns)
: columns_{ std::move(columns) }
{ }
SelectQuery::SelectQuery(ColumnsContainer columns, TablesContainer tables)
: columns_{ std::move(columns) }
, tables_{ std::move(tables) }
{}

SelectQuery::ExpectedQueryResult SelectQuery::resolve(sptr<CellValueGatherer> gatherer) {
    TableQueryResult::Table table(2);
    for(const Selector& column : columns_) { 
	table[0].push_back(column.column_name);
    }
    for(const Selector& column : columns_) {
	std::stringstream ss;
	auto result = column.content->resolve(gatherer);
	if(!result)
	    return std::unexpected(result.error());
	(*result)->format(ss);
	table[1].push_back(ss.str());
    }
    return std::make_shared<TableQueryResult>(std::move(table));
}

}
