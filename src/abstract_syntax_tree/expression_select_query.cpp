#include "expression_select_query.hpp"
#include "string_query_result.hpp"

namespace garlic {

ExpressionSelectQuery::ExpressionSelectQuery(sptr<Expression> expr)
: expression_ { std::move(expr) }
{}

sptr<QueryResult> ExpressionSelectQuery::resolve(sptr<TableValueGatherer> gatherer) {
    auto result = expression_->get_value(gatherer);
    std::stringstream ss;
    result->format(ss);
    return std::make_unique<StringQueryResult>(ss.str());
}

}
