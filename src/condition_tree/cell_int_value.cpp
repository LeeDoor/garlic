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
    return std::make_shared<CellFloatValue>(value_)->equals(other);
}
bool CellIntValue::le(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other))
        return get_int() <= to_type<IntType>(other);
    return std::make_shared<CellFloatValue>(value_)->le(other);
}
bool CellIntValue::lt(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other))
        return get_int() < to_type<IntType>(other);
    return std::make_shared<CellFloatValue>(value_)->lt(other);
}
bool CellIntValue::ge(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other))
        return get_int() >= to_type<IntType>(other);
    return std::make_shared<CellFloatValue>(value_)->ge(other);
}
bool CellIntValue::gt(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other))
        return get_int() > to_type<IntType>(other);
    return std::make_shared<CellFloatValue>(value_)->gt(other);
}

sptr<CellValue> CellIntValue::add(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
        return std::make_shared<CellIntValue>(value_ + to_type<IntType>(other));
    }
    return std::make_shared<CellFloatValue>(value_)->add(other);
}
sptr<CellValue> CellIntValue::sub(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
        return std::make_shared<CellIntValue>(value_ - to_type<IntType>(other));
    }
    return std::make_shared<CellFloatValue>(value_)->sub(other);
}
sptr<CellValue> CellIntValue::mul(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
        return std::make_shared<CellIntValue>(value_ * to_type<IntType>(other));
    }
    return std::make_shared<CellFloatValue>(value_)->mul(other);
}
sptr<CellValue> CellIntValue::div(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
        return std::make_shared<CellIntValue>(value_ / to_type<IntType>(other));
    }
    return std::make_shared<CellFloatValue>(value_)->div(other);
}
sptr<CellValue> CellIntValue::remdiv(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
	return std::make_shared<CellIntValue>(value_ % to_type<IntType>(other));
    }
    return std::make_shared<CellFloatValue>(value_)->remdiv(other);
}
sptr<CellValue> CellIntValue::abs() const {
    return std::make_shared<CellIntValue>(std::abs(value_));
}
sptr<CellValue> CellIntValue::neg() const {
    return std::make_shared<CellIntValue>(value_ * -1);
}

void CellIntValue::format(std::ostream& os) const {
    os << value_;
}

}
