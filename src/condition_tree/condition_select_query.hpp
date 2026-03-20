#pragma once
#include "cell_type.hpp"
#include "condition.hpp"
#include "query.hpp"

namespace garlic {

using RequiredColumns = ArrayType<StringType>;

class ConditionSelectQuery : public Query {
public:
    ConditionSelectQuery(Condition::Ptr condition);

    QueryResult::Ptr resolve(TableValueGatherer::Ptr gatherer) override;

private:
    Condition::Ptr condition_;
};

}
