#pragma once
#include "table_value_gatherer.hpp"

namespace garlic {

class Condition {
public:
    virtual ~Condition() = default;

    virtual bool resolve(sptr<TableValueGatherer> gatherer) const = 0;
};

}
