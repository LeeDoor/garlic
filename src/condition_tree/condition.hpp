#pragma once
#include "expression.hpp"
#include "table_value_gatherer.hpp"

namespace garlic {

class Condition {
protected:


public:
    virtual ~Condition() = default;

    virtual bool resolve(sptr<TableValueGatherer> gatherer) const = 0;
};

}

