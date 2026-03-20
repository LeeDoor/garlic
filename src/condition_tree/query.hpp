#pragma once

namespace garlic {
class QueryResult;
class TableValueGatherer;

class Query {
public:
    virtual ~Query() = default;

    virtual sptr<QueryResult> resolve(sptr<TableValueGatherer> gatherer) = 0;
};

}
