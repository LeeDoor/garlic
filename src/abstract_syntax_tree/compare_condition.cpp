#include "compare_condition.hpp"
#include "cell_value.hpp"
#include "expression.hpp"

namespace garlic {

CompareCondition::CompareCondition(sptr<Expression> lhs, sptr<Expression> rhs, BinaryOperator op)
: Condition { TypeRules::comparison_comp(lhs->get_type(), rhs->get_type()) }
, lhs_ { std::move(lhs) }
, rhs_{ std::move(rhs) }
, operator_  { op }
{}

bool CompareCondition::resolve(sptr<TableValueGatherer> gatherer) const {
    auto lhs = lhs_->get_value(gatherer),
	 rhs = rhs_->get_value(gatherer);
    switch(operator_) {
    case Eq:
	return lhs->equals(rhs);
    case Ne:
	return lhs->ne(rhs);
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

std::optional<StringType> CompareCondition::validate() const {
    return Validateable::validate(lhs_->get_type(), rhs_->get_type());
}

}
