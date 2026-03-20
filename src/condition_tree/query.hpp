#pragma once
#include "query_result.hpp"
#include "table_value_gatherer.hpp"

namespace garlic {

class Query {
public:
    virtual ~Query() = default;

    virtual sptr<QueryResult> resolve(sptr<TableValueGatherer> gatherer) = 0;
};

}
