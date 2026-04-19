#include "unary_logical_condition.hpp"
#include "cell_boolean_value.hpp"

namespace garlic {

UnaryLogicalCondition::UnaryLogicalCondition(sptr<Condition> condition, UnaryLogicalOperator op)
: Condition{ Boolean }
, cond_{ std::move(condition) }
, op_{ op }
{}

ExpectedCellValue UnaryLogicalCondition::resolve(sptr<TableValueGatherer> gatherer) const {
    auto value = cond_->resolve(gatherer); if(!value) return value;
    auto bool_value = std::dynamic_pointer_cast<CellBooleanValue>(*value);
    if(!bool_value)
	throw std::logic_error("Invalid unary logical operation on operand not allowing such actions"); 

    bool result;
    switch(op_) {
    case IsTrue:
	result = bool_value->get_bool(); break;
    case IsFalse:
	result = !bool_value->get_bool(); break;
    default:
	std::unreachable();
    }
    return std::make_shared<CellBooleanValue>(result);
}

}
