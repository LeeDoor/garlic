#include "select_query.hpp"
#include "string_query_result.hpp"

namespace garlic {

SelectQuery::SelectQuery()
: columns_{}
{}
SelectQuery::SelectQuery(ColumnsContainer columns)
: columns_{ std::move(columns) }
{ }

void SelectQuery::append_column(sptr<Expression> column_expression) {
    columns_.push_back(SelectColumn{ column_expression });
}
sptr<QueryResult> SelectQuery::resolve(sptr<TableValueGatherer> gatherer) {
    /// Complete mess free to edit
    std::stringstream ss;
    bool is_first = true;
    for(const SelectColumn& column : columns_) { 
	if(!is_first) {
	    ss << "\t";
	}
	is_first = false;

	ss << column.column_name; 
    }
    ss << std::endl;
    is_first = true;
    for(const SelectColumn& column : columns_) {
	if(!is_first) {
	    ss << "\t";
	}
	is_first = false;

	auto result = column.content->resolve(gatherer);
	if(!result)
	    return execute_error(result.error());
	(*result)->format(ss);
    }
    return std::make_shared<StringQueryResult>(ss.str());
}

}
