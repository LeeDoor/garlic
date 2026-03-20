#pragma once
#include "condition.hpp"
#include "binary_operator.hpp"

namespace garlic {

class CompareCondition : public Condition {
public:
    CompareCondition(ExpressionPtr lhs, ExpressionPtr rhs, BinaryOperator op);

    bool resolve(TableValueGathererPtr gatherer) const override;

private:
    ExpressionPtr expr_left_;
    ExpressionPtr expr_right_;
    BinaryOperator operator_;
};

}
