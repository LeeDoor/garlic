#pragma once
#include "condition.hpp"
#include "binary_operation.hpp"

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
        switch(operator_) {
        case Equals:
            return lhs->equals(rhs);
        case Gt:
            return lhs->gt(rhs);
        case Ge:
            return lhs->ge(rhs);
        case Lt:
            return lhs->lt(rhs);
        case Le:
            return lhs->le(rhs);
        }
    }

private:
    ExpressionPtr expr_left_;
    ExpressionPtr expr_right_;
    BinaryOperation operator_;
};

}
