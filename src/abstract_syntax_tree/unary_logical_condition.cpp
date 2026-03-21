#include "unary_logical_condition.hpp"

namespace garlic {

UnaryLogicalCondition::UnaryLogicalCondition(sptr<Condition> condition, UnaryLogicalOperator op)
: Condition{ Boolean }
, cond_{ std::move(condition) }
, op_{ op }
{}

bool UnaryLogicalCondition::resolve(sptr<TableValueGatherer> gatherer) const {
    switch(op_) {
    case IsTrue:
	return cond_->resolve(gatherer);
    case IsFalse:
	return !cond_->resolve(gatherer);
    }
    throw std::logic_error("UnaryLogicalCondition: not all switch cases populated");
}

}
