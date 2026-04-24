#include "select_query.hpp"
#include "table_query_result.hpp"

namespace garlic {

SelectQuery::SelectQuery()
: columns_{}
{}
SelectQuery::SelectQuery(ColumnsContainer columns)
: columns_{ std::move(columns) }
{ }

sptr<QueryResult> SelectQuery::resolve(sptr<TableValueGatherer> gatherer) {
    TableQueryResult::Table table; table.reserve(2); // TODO
    table.push_back({ });
    for(const Selector& column : columns_) { 
	table[0].push_back(column.column_name);
    }
    table.push_back({ });
    for(const Selector& column : columns_) {
	std::stringstream ss;
	auto result = column.content->resolve(gatherer);
	if(!result)
	    return execute_error(result.error());
	(*result)->format(ss);
	table[1].push_back(ss.str());
    }
    return std::make_shared<TableQueryResult>(std::move(table));
}

}
