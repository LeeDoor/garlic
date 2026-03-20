#include "cell_float_value.hpp"
#include "cell_int_value.hpp"

namespace garlic {
CellFloatValue::CellFloatValue(FloatType value)
: CellAcceptMathOp(Float)
, value_{ value }
{}

FloatType CellFloatValue::get_float() const { return value_; }

bool CellFloatValue::equals(CellValuePtr other) const {
    return fl_equals(other);
}
bool CellFloatValue::le(CellValuePtr other) const {
    return fl_equals(other) || get_float() <= to_float(other);
}
bool CellFloatValue::lt(CellValuePtr other) const {
    return !fl_equals(other) && get_float() < to_float(other);
}
bool CellFloatValue::ge(CellValuePtr other) const {
    return fl_equals(other) || get_float() >= to_float(other);
}
bool CellFloatValue::gt(CellValuePtr other) const {
    return !fl_equals(other) && get_float() > to_float(other);
}

CellValue::CellValuePtr CellFloatValue::add(CellValuePtr other) const {
    FloatType new_value = get_float() + to_float(other);
    return std::make_shared<CellFloatValue>(new_value);
}
CellValue::CellValuePtr CellFloatValue::sub(CellValuePtr other) const {
    FloatType new_value = get_float() - to_float(other);
    return std::make_shared<CellFloatValue>(new_value);
}
CellValue::CellValuePtr CellFloatValue::mul(CellValuePtr other) const {
    FloatType new_value = get_float() * to_float(other);
    return std::make_shared<CellFloatValue>(new_value);
}
CellValue::CellValuePtr CellFloatValue::div(CellValuePtr other) const {
    FloatType new_value = get_float() / to_float(other);
    return std::make_shared<CellFloatValue>(new_value);
}

void CellFloatValue::format(std::ostream& os) const {
    os << value_;
}
FloatType CellFloatValue::to_float(CellValuePtr other) const {
    auto float_ptr = std::dynamic_pointer_cast<CellFloatValue>(other);
    if(float_ptr != nullptr) 
        return float_ptr->get_float();
    auto int_ptr = std::dynamic_pointer_cast<CellIntValue>(other);
    if(int_ptr != nullptr) 
        return int_ptr->get_int();
    throw std::logic_error("Unable to convert \'other\' to float");
}

bool CellFloatValue::fl_equals(CellValuePtr other) const {
    FloatType e = std::numeric_limits<FloatType>::epsilon();
    return std::abs(get_float() - to_float(other)) < e;
}

}
