#pragma once
#include "expression.hpp"

namespace garlic {

template<typename ValueType>
class ConstantExpression : public Expression {
public:
    explicit ConstantExpression(ValueType value) 
    : value_(std::move(value))
    {}

    CellValuePtr get_value(TableValueGathererPtr) const override {
        return std::make_shared<get_cell_type<ValueType>::Type>(value_);
    }

protected:
    ValueType value_;
};

using IntConstExpr = ConstantExpression<IntType>;
using FloatConstExpr = ConstantExpression<FloatType>;
using StringConstExpr = ConstantExpression<StringViewType>;

}
