#pragma once
#include "cell_float_value.hpp"
#include "cell_int_value.hpp"
#include "cell_string_value.hpp"
#include "expression.hpp"

namespace garlic {

template<typename ValueType, typename CellValueType, CellType EnumType>
class ConstantExpression : public Expression {
public:
    explicit ConstantExpression(ResolverPtr resolver, ValueType value) 
    : Expression(EnumType, resolver)
    , value_(std::move(value))
    {}

    CellValuePtr get_value() const override {
        return std::make_shared<CellValueType>(value_);
    }

protected:
    ValueType value_;
};

using IntConstExpr = ConstantExpression<IntType, CellIntValue, Int>;
using FloatConstExpr = ConstantExpression<FloatType, CellFloatValue, Float>;
using StringConstExpr = ConstantExpression<StringViewType, CellStringValue, String>;

}
