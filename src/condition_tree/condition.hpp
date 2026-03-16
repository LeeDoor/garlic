#pragma once
#include "expression.hpp"
#include "table_value_gatherer.hpp"

namespace garlic {

class Condition {
protected:
    using ExpressionPtr = std::unique_ptr<Expression>;
    using TableValueGathererPtr = std::shared_ptr<TableValueGatherer>;
public:
    using Ptr = std::unique_ptr<Condition>;
    virtual ~Condition() = default;

    virtual bool resolve(TableValueGathererPtr gatherer) const = 0;
};

}

