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

CompareCondition::ExpectedBoolean CompareCondition::resolve(sptr<TableValueGatherer> gatherer) const {
    auto lvalue = lhs_->resolve(gatherer); if(!lvalue) return std::unexpected(std::move(lvalue.error()));
    auto rvalue = rhs_->resolve(gatherer); if(!rvalue) return std::unexpected(std::move(rvalue.error()));
    sptr<CellComparable> 
	lhs = std::dynamic_pointer_cast<CellComparable>(*lvalue),
	rhs = std::dynamic_pointer_cast<CellComparable>(*rvalue);

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
