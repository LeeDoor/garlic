#include "cell_int_value.hpp"
#include "cell_float_value.hpp"

namespace garlic {

CellIntValue::CellIntValue(IntType value)
: CellValue{ Int }
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

ExpectedCellValue CellIntValue::add(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
	IntType result;
	if(__builtin_add_overflow(value_, to_type<IntType>(other), &result))
	    return std::unexpected("Addition buffer overflow");
	return std::make_shared<CellIntValue>(result);
    }
    return std::make_shared<CellFloatValue>(value_)->add(other);
}
ExpectedCellValue CellIntValue::sub(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
	IntType result;
	if(__builtin_sub_overflow(value_, to_type<IntType>(other), &result))
	    return std::unexpected("Substraction buffer underflow");
	return std::make_shared<CellIntValue>(result);
    }
    return std::make_shared<CellFloatValue>(value_)->sub(other);
}
ExpectedCellValue CellIntValue::mul(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
	IntType result;
	if(__builtin_mul_overflow(value_, to_type<IntType>(other), &result))
	    return std::unexpected("Multiplication buffer underflow");
	return std::make_shared<CellIntValue>(result);
    }
    return std::make_shared<CellFloatValue>(value_)->mul(other);
}
ExpectedCellValue CellIntValue::div(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
	IntType rhs = to_type<IntType>(other);
	if(rhs == 0)
	    return std::unexpected("Division by zero");
	return std::make_shared<CellIntValue>(value_ / rhs);
    }
    return std::make_shared<CellFloatValue>(value_)->div(other);
}
ExpectedCellValue CellIntValue::remdiv(sptr<CellValue> other) const {
    if(is_type<CellIntValue>(other)) {
	IntType rhs = to_type<IntType>(other);
	if(rhs == 0)
	    return std::unexpected("Remainder of division by zero");
	return std::make_shared<CellIntValue>(value_ % rhs);
    }
    return std::make_shared<CellFloatValue>(value_)->remdiv(other);
}
ExpectedCellValue CellIntValue::abs() const {
    if(value_ == std::numeric_limits<IntType>::min())
	return std::unexpected("Absolute value of " + std::to_string(value_) + " does not fit in Integer type");
    return std::make_shared<CellIntValue>(std::abs(value_));
}
ExpectedCellValue CellIntValue::neg() const {
    if(value_ == std::numeric_limits<IntType>::min())
	return std::unexpected("Negative value of " + std::to_string(value_) + " does not fit in Integer type");
    return std::make_shared<CellIntValue>(value_ * -1);
}

void CellIntValue::format(std::ostream& os) const {
    os << value_;
}

}
