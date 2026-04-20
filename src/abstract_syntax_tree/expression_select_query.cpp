#include "expression_select_query.hpp"
#include "string_query_result.hpp"

namespace garlic {

ExpressionSelectQuery::ExpressionSelectQuery()
: columns_{}
{}
ExpressionSelectQuery::ExpressionSelectQuery(sptr<Expression> column_expression)
: columns_{}
{ columns_.push_back(create_column(column_expression)); }

void ExpressionSelectQuery::append_column(sptr<Expression> column_expression) {
    columns_.push_back(create_column(column_expression));
}
sptr<QueryResult> ExpressionSelectQuery::resolve(sptr<TableValueGatherer> gatherer) {
    /// Complete mess free to edit
    std::stringstream ss;
    bool is_first = true;
    for(const Column& column : columns_) { 
	if(!is_first) {
	    ss << "\t";
	}
	is_first = false;

	ss << column.column_name; 
    }
    ss << std::endl;
    is_first = true;
    for(const Column& column : columns_) {
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

ExpressionSelectQuery::Column ExpressionSelectQuery::create_column(sptr<Expression> column_content) {
    std::stringstream ss; TypeRules::as_str(ss, column_content->get_type());
    return Column{ ss.str(), column_content };
}

}
