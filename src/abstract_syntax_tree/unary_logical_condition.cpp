#include "unary_logical_condition.hpp"
#include "cell_boolean_value.hpp"

namespace garlic {

UnaryLogicalCondition::UnaryLogicalCondition(sptr<Condition> condition, UnaryLogicalOperator op)
: Condition{ Boolean }
, cond_{ std::move(condition) }
, op_{ op }
{}

UnaryLogicalCondition::ExpectedCellBooleanValue UnaryLogicalCondition::resolve_bool(const TablesGathered& gatherers) const {
    auto value = cond_->resolve_bool(gatherers); if(!value) return value;
    bool result;
    switch(op_) {
    case IsTrue:
	result = (*value)->get_bool(); break;
    case IsFalse:
	result = !(*value)->get_bool(); break;
    default:
	std::unreachable();
    }
    return std::make_shared<CellBooleanValue>(result);
}

UnaryLogicalCondition::UsedTables UnaryLogicalCondition::get_used_tables() const {
    return get_used_tables_from(cond_);
}

}
