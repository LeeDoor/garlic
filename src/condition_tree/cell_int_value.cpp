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

bool CellIntValue::equals(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other))
        return get_int() == to_type<IntType>(other);
    return make_copy<CellFloatValue>(value_)->equals(other);
}
bool CellIntValue::le(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other))
        return get_int() <= to_type<IntType>(other);
    return make_copy<CellFloatValue>(value_)->le(other);
}
bool CellIntValue::lt(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other))
        return get_int() < to_type<IntType>(other);
    return make_copy<CellFloatValue>(value_)->lt(other);
}
bool CellIntValue::ge(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other))
        return get_int() >= to_type<IntType>(other);
    return make_copy<CellFloatValue>(value_)->ge(other);
}
bool CellIntValue::gt(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other))
        return get_int() > to_type<IntType>(other);
    return make_copy<CellFloatValue>(value_)->gt(other);
}

sptr<CellValue> CellIntValue::add(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
        return make_copy<CellIntValue>(value_ + to_type<IntType>(other));
    }
    return make_copy<CellFloatValue>(value_ + to_type<FloatType>(other));
}
sptr<CellValue> CellIntValue::sub(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
        return make_copy<CellIntValue>(value_ - to_type<IntType>(other));
    }
    return make_copy<CellFloatValue>(value_ - to_type<FloatType>(other));
}
sptr<CellValue> CellIntValue::mul(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
        return make_copy<CellIntValue>(value_ * to_type<IntType>(other));
    }
    return make_copy<CellFloatValue>(value_ * to_type<FloatType>(other));
}
sptr<CellValue> CellIntValue::div(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
        return make_copy<CellIntValue>(value_ / to_type<IntType>(other));
    }
    return make_copy<CellFloatValue>(value_ / to_type<FloatType>(other));
}
void CellIntValue::format(std::ostream& os) const {
    os << value_;
}

}
