#pragma once
#include "cell_type.hpp"

namespace garlic {

class QueryResult {
protected:

public:
    virtual ~QueryResult() = default;

    virtual StringViewType format() const = 0;
};

}
