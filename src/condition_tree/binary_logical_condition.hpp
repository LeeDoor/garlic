#pragma once
#include "condition.hpp"
#include "logical_operator.hpp"

namespace garlic {

class BinaryLogicalCondition : public Condition {
protected:
    using LogicConditionPtr = std::shared_ptr<BinaryLogicalCondition>;
public:
    virtual ~BinaryLogicalCondition() = default;

    BinaryLogicalCondition(ConditionPtr lhs, ConditionPtr rhs, BinaryLogicalOperator op)
    : lhs_{ std::move(lhs) }
    , rhs_{ std::move(rhs) }
    , op_{ op }
    {}

    bool resolve(TableValueGathererPtr gatherer) const override {
        switch(op_) {
        case And:
            return lhs_->resolve(gatherer) && rhs_->resolve(gatherer);
        case Or:
            return lhs_->resolve(gatherer) || rhs_->resolve(gatherer);
        case Xor:
            return lhs_->resolve(gatherer) ^  rhs_->resolve(gatherer);
        case IfAndOnlyIf:
            return lhs_->resolve(gatherer) == rhs_->resolve(gatherer);
        }
        throw std::logic_error("BinaryLogicalCondition: not all switch cases populated");
    }

protected:
    ConditionPtr lhs_;
    ConditionPtr rhs_;
    BinaryLogicalOperator op_;
};

}
