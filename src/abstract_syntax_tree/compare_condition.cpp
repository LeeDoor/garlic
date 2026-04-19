#include "compare_condition.hpp"
#include "cell_boolean_value.hpp"
#include "cell_comparable.hpp"
#include "expression.hpp"

namespace garlic {

CompareCondition::CompareCondition(sptr<Expression> lhs, sptr<Expression> rhs, BinaryOperator op)
: Condition { TypeRules::comparison_comp(lhs->get_type(), rhs->get_type()) }
, lhs_ { std::move(lhs) }
, rhs_{ std::move(rhs) }
, operator_  { op }
{}

CompareCondition::ExpectedCellBooleanValue CompareCondition::resolve_bool(sptr<TableValueGatherer> gatherer) const {
    auto lvalue = lhs_->resolve(gatherer); if(!lvalue) return std::unexpected(lvalue.error());
    auto rvalue = rhs_->resolve(gatherer); if(!rvalue) return std::unexpected(rvalue.error());
    sptr<CellComparable> 
	lhs = std::dynamic_pointer_cast<CellComparable>(*lvalue),
	rhs = std::dynamic_pointer_cast<CellComparable>(*rvalue);

    bool result;
    switch(operator_) {
    case Eq:
	result = lhs->equals(rhs); break;
    case Ne:
	result = lhs->ne(rhs); break;
    case Gt:
	result = lhs->gt(rhs); break;
    case Ge:
	result = lhs->ge(rhs); break;
    case Lt:
	result = lhs->lt(rhs); break;
    case Le:
	result = lhs->le(rhs); break;
    default:
	std::unreachable();
    }
    return std::make_shared<CellBooleanValue>(result);
}

std::optional<StringType> CompareCondition::validate() const {
    return CanBeValidated::validate(lhs_->get_type(), rhs_->get_type());
}

}
