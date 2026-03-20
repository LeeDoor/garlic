#include "compare_condition.hpp"

namespace garlic {

CompareCondition::CompareCondition(ExpressionPtr lhs, ExpressionPtr rhs, BinaryOperator op)
: expr_left_ { std::move(lhs) }
, expr_right_{ std::move(rhs) }
, operator_  { op }
{}

bool CompareCondition::resolve(TableValueGathererPtr gatherer) const {
    auto lhs = expr_left_->get_value(gatherer),
	 rhs = expr_right_->get_value(gatherer);
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
	default:
	    throw std::logic_error("CompareCondition::resolve(gatherer) binary operator not implemented");
    }
}

}
