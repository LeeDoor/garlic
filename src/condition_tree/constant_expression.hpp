#pragma once
#include "expression.hpp"
#include "get_cell_type.hpp"
#include "cell_string_value.hpp"
#include "cell_float_value.hpp"
#include "cell_int_value.hpp"

namespace garlic {

template<IsStoringColumnType ValueType>
class ConstantExpression : public Expression {
public:
    ConstantExpression(ValueType value) 
    : value_(std::move(value))
    {}

    sptr<CellValue> get_value(sptr<TableValueGatherer>) const override {
        return std::make_shared<typename get_cell_type<ValueType>::Type>(value_);
    }

protected:
    ValueType value_;
};

using IntConstExpr = ConstantExpression<IntType>;
using FloatConstExpr = ConstantExpression<FloatType>;
using StringConstExpr = ConstantExpression<StringType>;

}
