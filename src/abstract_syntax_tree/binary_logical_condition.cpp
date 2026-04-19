#include "binary_logical_condition.hpp"
#include "cell_boolean_value.hpp"

namespace garlic {

BinaryLogicalCondition::BinaryLogicalCondition(sptr<Condition> lhs, sptr<Condition> rhs, BinaryLogicalOperator op)
: Condition{ Boolean }
, lhs_{ std::move(lhs) }
, rhs_{ std::move(rhs) }
, op_{ op }
{}

ExpectedCellValue BinaryLogicalCondition::resolve(sptr<TableValueGatherer> gatherer) const {
    auto lvalue = lhs_->resolve(gatherer); if(!lvalue) return lvalue;
    auto rvalue = rhs_->resolve(gatherer); if(!rvalue) return rvalue;
    sptr<CellBooleanValue> lhs = std::dynamic_pointer_cast<CellBooleanValue>(*lvalue);
    sptr<CellBooleanValue> rhs = std::dynamic_pointer_cast<CellBooleanValue>(*rvalue);
    if(!lhs || !rhs)
	throw std::logic_error("Invalid binary logical operation on operands not allowing such actions"); 

    bool result;
    switch(op_) {
	case And:
	    result = lhs->conjunction(rhs); break;
	case Or:
	    result = lhs->disjunction(rhs); break;
	case Xor:
	    result = lhs->exclusiveor(rhs); break;
	case IfAndOnlyIf:
	    result = lhs->equivalence(rhs); break;
	case Follows:
	    result = lhs->implication(rhs); break;
	default:
	    std::unreachable();
    }
    return std::make_shared<CellBooleanValue>(result);
}

}
