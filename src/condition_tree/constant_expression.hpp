#pragma once
#include "cell_float_value.hpp"
#include "cell_int_value.hpp"
#include "cell_string_value.hpp"
#include "expression.hpp"

namespace garlic {

template<typename ValueType, typename CellValueType>
class ConstantExpression : public Expression {
public:
    explicit ConstantExpression(ValueType value) 
    : value_(std::move(value))
    {}

    CellValuePtr get_value(TableValueGathererPtr) const override {
        return std::make_shared<CellValueType>(value_);
    }

protected:
    ValueType value_;
};

using IntConstExpr = ConstantExpression<IntType, CellIntValue>;
using FloatConstExpr = ConstantExpression<FloatType, CellFloatValue>;
using StringConstExpr = ConstantExpression<StringViewType, CellStringValue>;

}
