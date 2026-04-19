#pragma once
#include "condition.hpp"
#include "binary_operator.hpp"

namespace garlic {
class Expression;

class CompareCondition : public Condition {
public:
    CompareCondition(sptr<Expression> lhs, sptr<Expression> rhs, BinaryOperator op);

    ExpectedCellBooleanValue resolve_bool(sptr<TableValueGatherer> gatherer) const override;
    ExpectedValid validate() const override;

private:
    sptr<Expression> lhs_;
    sptr<Expression> rhs_;
    BinaryOperator operator_;
};

}
