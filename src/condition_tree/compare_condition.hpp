#pragma once
#include "condition.hpp"
#include "binary_operator.hpp"

namespace garlic {

class CompareCondition : public Condition {
public:
    CompareCondition(sptr<Expression> lhs, sptr<Expression> rhs, BinaryOperator op);

    bool resolve(sptr<TableValueGatherer> gatherer) const override;

private:
    sptr<Expression> expr_left_;
    sptr<Expression> expr_right_;
    BinaryOperator operator_;
};

}
