#include "select_query.hpp"
#include "table_query_result.hpp"
#include "dumb_cell_value_gatherer.hpp"
#include "table_value_gatherer_factory.hpp"
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

SelectQuery::ExpectedQueryResult SelectQuery::resolve(const TableValueGathererFactory& gatherer_factory) {
    TablesGathered gatherers {};
    for(const Table& table : tables_) {
	auto exp_gatherer = gatherer_factory.build_cell_value_gatherer(table.table_name);
	if(!exp_gatherer)
	    return std::unexpected(exp_gatherer.error());
	gatherers[table.table_name] = *exp_gatherer;
    }

    TableQueryResult::Table table(2);
    for(const Selector& column : columns_) { 
	table[0].push_back(column.column_name);
    }
    for(const Selector& column : columns_) {
	std::stringstream ss;
	auto result = column.content->resolve(gatherers);
	if(!result)
	    return std::unexpected(result.error());
	(*result)->format(ss);
	table[1].push_back(ss.str());
    }
    return std::make_shared<TableQueryResult>(std::move(table));
}

}
