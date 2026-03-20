#pragma once
#include "cell_type.hpp"
#include "condition.hpp"
#include "query.hpp"

namespace garlic {

using RequiredColumns = ArrayType<StringType>;

class ConditionSelectQuery : public Query {
public:
    ConditionSelectQuery(sptr<Condition> condition);

    sptr<QueryResult> resolve(sptr<TableValueGatherer> gatherer) override;

private:
    sptr<Condition> condition_;
};

}
