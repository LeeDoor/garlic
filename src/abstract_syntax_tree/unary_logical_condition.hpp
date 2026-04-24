#pragma once
#include "condition.hpp"
#include "logical_operator.hpp"

namespace garlic {

class UnaryLogicalCondition : public Condition {
public:
    UnaryLogicalCondition(sptr<Condition> condition, UnaryLogicalOperator op);

    ExpectedCellBooleanValue resolve_bool(sptr<TableValueGatherer> gatherer) const override;

protected:
    sptr<Condition> cond_;
    UnaryLogicalOperator op_;
};

}
