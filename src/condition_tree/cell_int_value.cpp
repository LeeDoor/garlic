#include "cell_int_value.hpp"
#include "cell_float_value.hpp"

namespace garlic {

CellIntValue::CellIntValue(IntType value)
: CellValue(Int)
, value_{ value }
{}

IntType CellIntValue::get_int() const { return value_; }
FloatType CellIntValue::get_float() const { return static_cast<FloatType>(get_int()); }

bool CellIntValue::equals(CellValuePtr other) const {
    if(is_int(other))
        return get_int() == to_int(other);
    return as_float()->equals(other);
}
bool CellIntValue::le(CellValuePtr other) const {
    if(is_int(other))
        return get_int() <= to_int(other);
    return as_float()->le(other);
}
bool CellIntValue::lt(CellValuePtr other) const {
    if(is_int(other))
        return get_int() < to_int(other);
    return as_float()->lt(other);
}
bool CellIntValue::ge(CellValuePtr other) const {
    if(is_int(other))
        return get_int() >= to_int(other);
    return as_float()->ge(other);
}
bool CellIntValue::gt(CellValuePtr other) const {
    if(is_int(other))
        return get_int() > to_int(other);
    return as_float()->gt(other);
}

CellIntValue::CellValuePtr CellIntValue::add(CellValuePtr other) const {
    if(is_int(other)) {
        return std::make_shared<CellIntValue>(get_int() + to_int(other));
    }
    FloatType other_float = std::dynamic_pointer_cast<CellFloatValue>(other)->get_float();
    return std::make_shared<CellFloatValue>(get_float() + other_float);
}
CellIntValue::CellValuePtr CellIntValue::sub(CellValuePtr other) const {

}
CellIntValue::CellValuePtr CellIntValue::mul(CellValuePtr other) const {

}
CellIntValue::CellValuePtr CellIntValue::div(CellValuePtr other) const {

}
void CellIntValue::format(std::ostream& os) const {
    os << value_;
}

bool CellIntValue::is_int(CellValuePtr other) {
    auto int_ptr = std::dynamic_pointer_cast<CellIntValue>(other);
    return int_ptr != nullptr;
}
IntType CellIntValue::to_int(CellValuePtr other) {
    auto int_ptr = std::dynamic_pointer_cast<CellIntValue>(other);
    if(int_ptr == nullptr) 
        throw std::logic_error("Casting non-integer to int");
    return int_ptr->get_int();
}
bool CellIntValue::is_float(CellValuePtr other) {

}
static FloatType CellIntValue::to_float(CellValuePtr other);
std::unique_ptr<CellFloatValue> CellIntValue::as_float() const {
    FloatType as_float = static_cast<FloatType>(get_int());
    return std::make_unique<CellFloatValue>(as_float);
}

}
