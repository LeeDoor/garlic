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
    ResultTable result_table = create_result_table_header();
    bool has_empty_table = false;
    auto gatherers = create_gatherers(gatherer_factory, has_empty_table);
    if(has_empty_table) 
	return std::make_shared<TableQueryResult>(std::move(result_table));
    if(!gatherers) 
	return std::unexpected(gatherers.error());
    auto [gatherers_hash, gatherers_ordered] = std::move(*gatherers);
    do {
	auto row = resolve_row(gatherers_hash);
	if(!row) return std::unexpected(row.error());
	result_table.push_back(*row);
    } while(jump_to_next_row(gatherers_ordered));
    return std::make_shared<TableQueryResult>(std::move(result_table));
}

ResultTable SelectQuery::create_result_table_header() const {
    ResultTable result_table(1);
    for(const Selector& column : columns_) { 
	result_table[0].push_back(column.column_name);
    }
    return result_table;
}

std::expected<std::pair<TablesGathered, SelectQuery::OrderedGatherers>, UnexpectedCellValue> 
SelectQuery::create_gatherers(const TableValueGathererFactory& gatherer_factory, bool& has_empty_table) const {
    has_empty_table = false;
    TablesGathered gatherers_hash {};
    OrderedGatherers gatherers_ordered {};
    for(const Table& selected_table : tables_) {
	auto exp_gatherer = gatherer_factory.build_cell_value_gatherer(selected_table.table_name);
	if(!exp_gatherer)
	    return std::unexpected(exp_gatherer.error());
	if((*exp_gatherer)->is_table_empty()) {
	    has_empty_table = true;
	    return std::pair(gatherers_hash, gatherers_ordered);
	}
	gatherers_hash[selected_table.table_name] = *exp_gatherer;
	gatherers_ordered.push_back(*exp_gatherer);
    }
    return std::pair(gatherers_hash, gatherers_ordered);
}

bool SelectQuery::jump_to_next_row(OrderedGatherers& gatherers) {
    auto iter = gatherers.rbegin();
    while(iter != gatherers.rend() && (*iter)->jump_to_next_row()) {
	++iter;
    }
    return iter != gatherers.rend();
}

std::expected<ResultRow, UnexpectedCellValue> SelectQuery::resolve_row(const TablesGathered& gatherers) const {
    ResultRow result_row; result_row.reserve(columns_.size());
    for(const Selector& column : columns_) {
	auto resolved = resolve_and_stringfy(column, gatherers);
	if(!resolved) return std::unexpected(resolved.error());
	result_row.push_back(*resolved);
    }
    return result_row;
}

std::expected<StringType, UnexpectedCellValue> SelectQuery::resolve_and_stringfy(const Selector& column, const TablesGathered& gatherers) const {
    std::stringstream ss;
    auto result = column.ast->resolve(gatherers);
    if(!result)
	return std::unexpected(result.error());
    (*result)->format(ss);
    return ss.str();
}

}
