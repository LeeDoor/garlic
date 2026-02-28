#pragma once
#include "condition.hpp"
#include "logical_operator.hpp"

namespace garlic {

class UnaryLogicalCondition : public Condition {
protected:
    using UnaryLogicalConditionPtr = std::shared_ptr<UnaryLogicalCondition>;
public:
    virtual ~UnaryLogicalCondition() = default;

    UnaryLogicalCondition(ConditionPtr condition, UnaryLogicalOperator op)
    : cond_{ std::move(condition) }
    , op_{ op }
    {}

    bool resolve(TableValueGathererPtr gatherer) const override {
        switch(op_) {
        case IsTrue:
            return cond_->resolve(gatherer);
        case IsFalse:
            return !cond_->resolve(gatherer);
        }
        throw std::logic_error("UnaryLogicalCondition: not all switch cases populated");
    }

protected:
    ConditionPtr cond_;
    UnaryLogicalOperator op_;
};

}
