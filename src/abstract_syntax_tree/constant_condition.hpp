#pragma once
#include "condition.hpp"

namespace garlic {

class ConstantCondition : public Condition {
    using ValueType = bool;
public:
    ConstantCondition(ValueType value) 
    : Condition{ TypeRules::get_cell_from_type<ValueType>() }
    , value_(std::move(value))
    {}

    ExpectedValid validate() const override { return ExpectedValid{}; }

    ExpectedCellBooleanValue resolve_bool(sptr<TableValueGatherer>) const override {
        return std::make_shared<typename get_cell_type<ValueType>::Type>(value_);
    }

protected:
    ValueType value_;
};

}
