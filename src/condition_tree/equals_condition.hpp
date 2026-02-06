#pragma once
#include "condition.hpp"

namespace garlic {

class CompareCondition : public Condition {
public:
    CompareCondition(ResolverPtr resolver, ExpressionPtr lhs, ExpressionPtr rhs, BinaryOperation op)
    : Condition(  resolver )
    , expr_left_ { std::move(lhs) }
    , expr_right_{ std::move(rhs) }
    , operator_  { op }
    {}

    bool resolve() override {
        auto lhs = expr_left_->get_value(),
             rhs = expr_right_->get_value();
        return lhs->compare(rhs, operator_);
    }

private:
    ExpressionPtr expr_left_;
    ExpressionPtr expr_right_;
    BinaryOperation operator_;
};

}
