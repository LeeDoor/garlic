#pragma once
#include "cell_type.hpp"
#include "condition.hpp"
#include "query.hpp"
#include "string_query_result.hpp"

namespace garlic {

using RequiredColumns = ArrayType<StringType>;

class ConditionSelectQuery : public Query {
public:
    virtual ~ConditionSelectQuery() = default;
    
    ConditionSelectQuery(Condition::Ptr condition)
    : condition_{ std::move(condition) }
    {}

    QueryResult::Ptr resolve(TableValueGatherer::Ptr gatherer) override {
	auto result = condition_->resolve(gatherer);
	return std::make_unique<StringQueryResult>(static_cast<int>(result));
    }

private:
    Condition::Ptr condition_;
};

}
