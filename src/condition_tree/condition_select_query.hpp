#pragma once
#include "condition.hpp"
#include "query.hpp"

namespace garlic {

class ConditionSelectQuery : public Query {
public:
    ConditionSelectQuery(sptr<Condition> condition);

    sptr<QueryResult> resolve(sptr<TableValueGatherer> gatherer) override;

private:
    sptr<Condition> condition_;
};

}
