#include "select_query.hpp"
#include "table_query_result.hpp"
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
    TableQueryResult::Table result_table(1);
    for(const Selector& column : columns_) { 
	result_table[0].push_back(column.column_name);
    }
    TablesGathered gatherers_hash {};
    std::list<sptr<CellValueGatherer>> gatherers_ordered {};
    for(const Table& selected_table : tables_) {
	auto exp_gatherer = gatherer_factory.build_cell_value_gatherer(selected_table.table_name);
	if(!exp_gatherer)
	    return std::unexpected(exp_gatherer.error());
	if((*exp_gatherer)->is_table_empty())
	    return std::make_shared<TableQueryResult>(std::move(result_table));
	gatherers_hash[selected_table.table_name] = *exp_gatherer;
	gatherers_ordered.push_back(*exp_gatherer);
    }

    while(true) {
	result_table.push_back({});
	for(const Selector& column : columns_) {
	    std::stringstream ss;
	    auto result = column.content->resolve(gatherers_hash);
	    if(!result)
		return std::unexpected(result.error());
	    (*result)->format(ss);
	    result_table.back().push_back(ss.str());
	}
	if(gatherers_ordered.empty()) break;
	auto iter = gatherers_ordered.rbegin();
	while(iter != gatherers_ordered.rend() && (*iter)->jump_to_next_row()) {
	    ++iter;
	}
	if(iter == gatherers_ordered.rend()) break;
    }
    return std::make_shared<TableQueryResult>(std::move(result_table));
}

}
