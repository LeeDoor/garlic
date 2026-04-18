#include "compare_condition.hpp"
#include "cell_comparable.hpp"
#include "expression.hpp"

namespace garlic {

CompareCondition::CompareCondition(sptr<Expression> lhs, sptr<Expression> rhs, BinaryOperator op)
: Condition { TypeRules::comparison_comp(lhs->get_type(), rhs->get_type()) }
, lhs_ { std::move(lhs) }
, rhs_{ std::move(rhs) }
, operator_  { op }
{}

bool CompareCondition::resolve(sptr<TableValueGatherer> gatherer) const {
    sptr<CellComparable> 
	lhs = std::dynamic_pointer_cast<CellComparable>(lhs_->get_value(gatherer)),
	rhs = std::dynamic_pointer_cast<CellComparable>(rhs_->get_value(gatherer));
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
	std::unreachable();
    }
}

std::optional<StringType> CompareCondition::validate() const {
    return CanBeValidated::validate(lhs_->get_type(), rhs_->get_type());
}

}
