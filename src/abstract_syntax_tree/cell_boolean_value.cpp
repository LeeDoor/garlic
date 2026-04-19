#include "cell_boolean_value.hpp"    

namespace garlic {

CellBooleanValue::CellBooleanValue(bool bool_value)
: CellValue{ Boolean } 
, value_{ bool_value }
{}

void CellBooleanValue::format(std::ostream& os) const {
    os << std::boolalpha << value_;
}

CellBooleanValue::operator bool() const {
    return value_;
}
bool CellBooleanValue::get_bool() const {
    return value_;
}

bool CellBooleanValue::conjunction(sptr<CellBooleanValue> other) const {
    return value_ && other->get_bool();
}
bool CellBooleanValue::disjunction(sptr<CellBooleanValue> other) const {
    return value_ || other->get_bool();
}
bool CellBooleanValue::equivalence(sptr<CellBooleanValue> other) const {
    return value_ == other->get_bool();
}
bool CellBooleanValue::implication(sptr<CellBooleanValue> other) const {
    return value_ <= other->get_bool();
}
bool CellBooleanValue::exclusiveor(sptr<CellBooleanValue> other) const {
    return value_ ^  other->get_bool();
}

}
