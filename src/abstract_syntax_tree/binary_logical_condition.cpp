#include "binary_logical_condition.hpp"

namespace garlic {

BinaryLogicalCondition::BinaryLogicalCondition(sptr<Condition> lhs, sptr<Condition> rhs, BinaryLogicalOperator op)
: Condition{ Boolean }
, lhs_{ std::move(lhs) }
, rhs_{ std::move(rhs) }
, op_{ op }
{}

BinaryLogicalCondition::ExpectedBoolean BinaryLogicalCondition::resolve(sptr<TableValueGatherer> gatherer) const {
    auto lvalue = lhs_->resolve(gatherer); if(!lvalue) return lvalue;
    auto rvalue = rhs_->resolve(gatherer); if(!rvalue) return rvalue;
    switch(op_) {
	case And:
	    return *lvalue && *rvalue;
	case Or:
	    return *lvalue || *rvalue;
	case Xor:
	    return *lvalue ^  *rvalue;
	case IfAndOnlyIf:
	    return *lvalue == *rvalue;
    }
    throw std::logic_error("BinaryLogicalCondition: not all switch cases populated");
}

}
