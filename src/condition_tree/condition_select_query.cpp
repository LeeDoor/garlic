#include "condition_select_query.hpp"
#include "string_query_result.hpp"

namespace garlic {

ConditionSelectQuery::ConditionSelectQuery(Condition::Ptr condition)
: condition_{ std::move(condition) }
{}

QueryResult::Ptr ConditionSelectQuery::resolve(TableValueGatherer::Ptr gatherer) {
    auto result = condition_->resolve(gatherer);
    return std::make_unique<StringQueryResult>(static_cast<int>(result));
}

}
