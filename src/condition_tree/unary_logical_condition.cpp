#include "unary_logical_condition.hpp"

namespace garlic {

UnaryLogicalCondition::UnaryLogicalCondition(Condition::Ptr condition, UnaryLogicalOperator op)
: cond_{ std::move(condition) }
, op_{ op }
{}

bool UnaryLogicalCondition::resolve(TableValueGathererPtr gatherer) const {
    switch(op_) {
    case IsTrue:
	return cond_->resolve(gatherer);
    case IsFalse:
	return !cond_->resolve(gatherer);
    }
    throw std::logic_error("UnaryLogicalCondition: not all switch cases populated");
}

}
