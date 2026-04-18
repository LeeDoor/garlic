#include "unary_logical_condition.hpp"

namespace garlic {

UnaryLogicalCondition::UnaryLogicalCondition(sptr<Condition> condition, UnaryLogicalOperator op)
: Condition{ Boolean }
, cond_{ std::move(condition) }
, op_{ op }
{}

UnaryLogicalCondition::ExpectedBoolean UnaryLogicalCondition::resolve(sptr<TableValueGatherer> gatherer) const {
    auto value = cond_->resolve(gatherer); if(!value) return value;
    switch(op_) {
    case IsTrue:
	return value.value();
    case IsFalse:
	return !value.value();
    }
    throw std::logic_error("UnaryLogicalCondition: not all switch cases populated");
}

}
