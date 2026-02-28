#pragma once
#include "condition.hpp"

namespace garlic {

class BinaryLogicalCondition : public Condition {
protected:
    using LogicConditionPtr = std::shared_ptr<BinaryLogicalCondition>;
public:
    virtual ~BinaryLogicalCondition() = default;

    BinaryLogicalCondition(ConditionPtr lhs, ConditionPtr rhs, LogicOperator op)
    : lhs_{ lhs }
    , rhs_{ rhs }
    , op_{ op }
    {}

    bool resolve(TableValueGathererPtr gatherer) const override {
        
    }
};

}
