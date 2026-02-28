#pragma once
#include "expression.hpp"
#include "get_cell_type.hpp"

namespace garlic {

template<IsStoringColumnType ValueType>
class ConstantExpression : public Expression {
public:
    ConstantExpression(ValueType value) 
    : value_(std::move(value))
    {}

    CellValuePtr get_value(TableValueGathererPtr) const override {
        return std::make_shared<typename get_cell_type<ValueType>::Type>(value_);
    }

protected:
    ValueType value_;
};

using IntConstExpr = ConstantExpression<IntType>;
using FloatConstExpr = ConstantExpression<FloatType>;
using StringConstExpr = ConstantExpression<StringType>;

}
