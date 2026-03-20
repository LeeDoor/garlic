#pragma once
#include "condition.hpp"
#include "logical_operator.hpp"

namespace garlic {

class UnaryLogicalCondition : public Condition {
protected:
    using UnaryLogicalConditionPtr = std::shared_ptr<UnaryLogicalCondition>;
public:
    UnaryLogicalCondition(Condition::Ptr condition, UnaryLogicalOperator op);

    bool resolve(TableValueGathererPtr gatherer) const override;

protected:
    Condition::Ptr cond_;
    UnaryLogicalOperator op_;
};

}
