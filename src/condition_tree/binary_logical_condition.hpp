#pragma once
#include "condition.hpp"
#include "logical_operator.hpp"

namespace garlic {

class BinaryLogicalCondition : public Condition {
protected:
    using LogicConditionPtr = std::shared_ptr<BinaryLogicalCondition>;
public:
    BinaryLogicalCondition(Condition::Ptr lhs, Condition::Ptr rhs, BinaryLogicalOperator op);

    bool resolve(TableValueGathererPtr gatherer) const override;

protected:
    Condition::Ptr lhs_;
    Condition::Ptr rhs_;
    BinaryLogicalOperator op_;
};

}
