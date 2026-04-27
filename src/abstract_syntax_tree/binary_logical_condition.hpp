#pragma once
#include "condition.hpp"
#include "logical_operator.hpp"

namespace garlic {

class BinaryLogicalCondition : public Condition {
public:
    BinaryLogicalCondition(sptr<Condition> lhs, sptr<Condition> rhs, BinaryLogicalOperator op);

    ExpectedCellBooleanValue resolve_bool(const TablesGathered& gatherers) const override;

protected:
    sptr<Condition> lhs_;
    sptr<Condition> rhs_;
    BinaryLogicalOperator op_;
};

}
