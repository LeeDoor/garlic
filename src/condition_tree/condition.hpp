#pragma once
#include "expression.hpp"
#include "table_value_gatherer.hpp"

namespace garlic {

class Condition {
protected:
    using ExpressionPtr = std::unique_ptr<Expression>;
    using TableValueGathererPtr = std::shared_ptr<TableValueGatherer>;
public:
    virtual bool resolve(TableValueGathererPtr gatherer) = 0;

protected:
    Condition() {}
};

}

