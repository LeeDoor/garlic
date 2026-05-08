#include "binary_logical_condition.hpp"
#include "cell_boolean_value.hpp"

namespace garlic {

BinaryLogicalCondition::BinaryLogicalCondition(sptr<Condition> lhs, sptr<Condition> rhs, BinaryLogicalOperator op)
: Condition{ Boolean }
, lhs_{ std::move(lhs) }
, rhs_{ std::move(rhs) }
, op_{ op }
{}

BinaryLogicalCondition::ExpectedCellBooleanValue BinaryLogicalCondition::resolve_bool(const TablesGathered& gatherers) const {
    auto lhs = lhs_->resolve_bool(gatherers); if(!lhs) return std::unexpected(lhs.error());
    auto rhs = rhs_->resolve_bool(gatherers); if(!rhs) return std::unexpected(rhs.error());
    bool result;
    switch(op_) {
	case And:
	    result = (*lhs)->conjunction(*rhs); break;
	case Or:
	    result = (*lhs)->disjunction(*rhs); break;
	case Xor:
	    result = (*lhs)->exclusiveor(*rhs); break;
	case Iff:
	    result = (*lhs)->equivalence(*rhs); break;
	case Implication:
	    result = (*lhs)->implication(*rhs); break;
	default:
	    std::unreachable();
    }
    return std::make_shared<CellBooleanValue>(result);
}
BinaryLogicalCondition::UsedTables BinaryLogicalCondition::get_used_tables() const {
    return get_used_tables_from(lhs_, rhs_);
}

}
