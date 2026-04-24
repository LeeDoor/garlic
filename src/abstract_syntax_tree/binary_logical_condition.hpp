#pragma once
#include "condition.hpp"
#include "logical_operator.hpp"

namespace garlic {

class BinaryLogicalCondition : public Condition {
public:
    BinaryLogicalCondition(sptr<Condition> lhs, sptr<Condition> rhs, BinaryLogicalOperator op);

    ExpectedCellBooleanValue resolve_bool(sptr<TableValueGatherer> gatherer) const override;

protected:
    sptr<Condition> lhs_;
    sptr<Condition> rhs_;
    BinaryLogicalOperator op_;
};

}
