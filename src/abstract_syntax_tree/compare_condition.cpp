#include "compare_condition.hpp"
#include "type_rules.hpp"
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

CompareCondition::ExpectedCellBooleanValue CompareCondition::resolve_bool(const TablesGathered& gatherers) const {
    auto lvalue = lhs_->resolve(gatherers); if(!lvalue) return std::unexpected(lvalue.error());
    auto rvalue = rhs_->resolve(gatherers); if(!rvalue) return std::unexpected(rvalue.error());
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
CompareCondition::UsedTables CompareCondition::get_used_tables() const {
    return get_used_tables_from(lhs_, rhs_);
}

}
