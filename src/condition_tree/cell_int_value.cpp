#include "cell_int_value.hpp"
#include "cell_float_value.hpp"

namespace garlic {

CellIntValue::CellIntValue(IntType value)
: CellAcceptMathOp(Int)
, value_{ value }
{}

CellIntValue::operator IntType() const {
    return value_;
}
IntType CellIntValue::get_int() const { return value_; }
FloatType CellIntValue::get_float() const { return static_cast<FloatType>(get_int()); }

bool CellIntValue::equals(CellValuePtr other) const {
    if(is_type<CellIntValue>(other))
        return get_int() == to_type<IntType>(other);
    return make_copy<CellFloatValue>(value_)->equals(other);
}
bool CellIntValue::le(CellValuePtr other) const {
    if(is_type<CellIntValue>(other))
        return get_int() <= to_type<IntType>(other);
    return make_copy<CellFloatValue>(value_)->le(other);
}
bool CellIntValue::lt(CellValuePtr other) const {
    if(is_type<CellIntValue>(other))
        return get_int() < to_type<IntType>(other);
    return make_copy<CellFloatValue>(value_)->lt(other);
}
bool CellIntValue::ge(CellValuePtr other) const {
    if(is_type<CellIntValue>(other))
        return get_int() >= to_type<IntType>(other);
    return make_copy<CellFloatValue>(value_)->ge(other);
}
bool CellIntValue::gt(CellValuePtr other) const {
    if(is_type<CellIntValue>(other))
        return get_int() > to_type<IntType>(other);
    return make_copy<CellFloatValue>(value_)->gt(other);
}

CellIntValue::CellValuePtr CellIntValue::add(CellValuePtr other) const {
    if(is_type<CellIntValue>(other)) {
        return make_copy<CellIntValue>(value_ + to_type<IntType>(other));
    }
    return make_copy<CellFloatValue>(value_ + to_type<FloatType>(other));
}
CellIntValue::CellValuePtr CellIntValue::sub(CellValuePtr other) const {
    if(is_type<CellIntValue>(other)) {
        return make_copy<CellIntValue>(value_ - to_type<IntType>(other));
    }
    return make_copy<CellFloatValue>(value_ - to_type<FloatType>(other));
}
CellIntValue::CellValuePtr CellIntValue::mul(CellValuePtr other) const {
    if(is_type<CellIntValue>(other)) {
        return make_copy<CellIntValue>(value_ * to_type<IntType>(other));
    }
    return make_copy<CellFloatValue>(value_ * to_type<FloatType>(other));
}
CellIntValue::CellValuePtr CellIntValue::div(CellValuePtr other) const {
    if(is_type<CellIntValue>(other)) {
        return make_copy<CellIntValue>(value_ / to_type<IntType>(other));
    }
    return make_copy<CellFloatValue>(value_ / to_type<FloatType>(other));
}
void CellIntValue::format(std::ostream& os) const {
    os << value_;
}

}
