#include "cell_float_value.hpp"
#include "cell_int_value.hpp"

namespace garlic {
CellFloatValue::CellFloatValue(FloatType value)
: CellAcceptMathOp(Float)
, value_{ value }
{}

CellFloatValue::operator FloatType() const {
    return value_;
}
FloatType CellFloatValue::get_float() const { return value_; }

bool CellFloatValue::equals(sptr<CellValue> other) const {
    return fl_equals(other);
}
bool CellFloatValue::le(sptr<CellValue> other) const {
    return fl_equals(other) || get_float() <= number_to_float(other);
}
bool CellFloatValue::lt(sptr<CellValue> other) const {
    return !fl_equals(other) && get_float() < number_to_float(other);
}
bool CellFloatValue::ge(sptr<CellValue> other) const {
    return fl_equals(other) || get_float() >= number_to_float(other);
}
bool CellFloatValue::gt(sptr<CellValue> other) const {
    return !fl_equals(other) && get_float() > number_to_float(other);
}

sptr<CellValue> CellFloatValue::add(sptr<CellValue> other) const {
    FloatType new_value = get_float() + number_to_float(other);
    return std::make_shared<CellFloatValue>(new_value);
}
sptr<CellValue> CellFloatValue::sub(sptr<CellValue> other) const {
    FloatType new_value = get_float() - number_to_float(other);
    return std::make_shared<CellFloatValue>(new_value);
}
sptr<CellValue> CellFloatValue::mul(sptr<CellValue> other) const {
    FloatType new_value = get_float() * number_to_float(other);
    return std::make_shared<CellFloatValue>(new_value);
}
sptr<CellValue> CellFloatValue::div(sptr<CellValue> other) const {
    FloatType new_value = get_float() / number_to_float(other);
    return std::make_shared<CellFloatValue>(new_value);
}
sptr<CellValue> CellFloatValue::remdiv(sptr<CellValue> other) const {
    FloatType new_value = std::fmod(get_float(), number_to_float(other));
    return std::make_shared<CellFloatValue>(new_value);
}
sptr<CellValue> CellFloatValue::abs() const {
    return std::make_shared<CellFloatValue>(std::fabs(value_));
}
sptr<CellValue> CellFloatValue::neg() const {
    return std::make_shared<CellFloatValue>(value_ * -1.0f);
}

void CellFloatValue::format(std::ostream& os) const {
    os << value_;
}
FloatType CellFloatValue::number_to_float(sptr<CellValue> other) {
    if(is_type<CellFloatValue>(other))
	return to_type<FloatType>(other);
    if(is_type<CellIntValue>(other))
	return static_cast<FloatType>(to_type<IntType>(other));
    throw std::logic_error("Unable to convert \'other\' to float");
}

bool CellFloatValue::fl_equals(sptr<CellValue> other) const {
    FloatType e = std::numeric_limits<FloatType>::epsilon();
    return std::abs(get_float() - number_to_float(other)) < e;
}

}
