#pragma once
#include "query_result.hpp"
#include "table_value_gatherer.hpp"

namespace garlic {

class Query {
public:
   using Ptr = std::shared_ptr<Query>;
   virtual ~Query() = default;

    virtual QueryResult::Ptr resolve(TableValueGatherer::Ptr gatherer) = 0;
};

}
