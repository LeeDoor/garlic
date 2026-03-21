#include "binary_logical_condition.hpp"

namespace garlic {

BinaryLogicalCondition::BinaryLogicalCondition(sptr<Condition> lhs, sptr<Condition> rhs, BinaryLogicalOperator op)
: Condition{ Boolean }
, lhs_{ std::move(lhs) }
, rhs_{ std::move(rhs) }
, op_{ op }
{}

bool BinaryLogicalCondition::resolve(sptr<TableValueGatherer> gatherer) const {
    switch(op_) {
	case And:
	    return lhs_->resolve(gatherer) && rhs_->resolve(gatherer);
	case Or:
	    return lhs_->resolve(gatherer) || rhs_->resolve(gatherer);
	case Xor:
	    return lhs_->resolve(gatherer) ^  rhs_->resolve(gatherer);
	case IfAndOnlyIf:
	    return lhs_->resolve(gatherer) == rhs_->resolve(gatherer);
    }
    throw std::logic_error("BinaryLogicalCondition: not all switch cases populated");
}

}
