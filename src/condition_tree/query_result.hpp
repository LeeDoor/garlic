#pragma once
#include "cell_type.hpp"

namespace garlic {

class QueryResult {
protected:
public:
    using Ptr = std::shared_ptr<QueryResult>;
    virtual ~QueryResult() = default;

    virtual StringViewType format() const = 0;
};

}
