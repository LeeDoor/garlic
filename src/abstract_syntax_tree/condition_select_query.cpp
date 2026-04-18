#include "condition_select_query.hpp"
#include "string_query_result.hpp"

namespace garlic {

ConditionSelectQuery::ConditionSelectQuery(sptr<Condition> condition)
: condition_{ std::move(condition) }
{}

sptr<QueryResult> ConditionSelectQuery::resolve(sptr<TableValueGatherer> gatherer) {
    auto result = condition_->resolve(gatherer);
    if(!result)	return execute_error(result.error());
    return std::make_unique<StringQueryResult>(static_cast<int>(*result));
}

}
