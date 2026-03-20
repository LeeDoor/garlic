#include "cell_string_value.hpp"

namespace garlic {

CellStringValue::CellStringValue(StringViewType value)
    : CellValue(String)
      , value_{ value }
{}

CellStringValue::operator StringViewType() const {
    return value_;
}

StringViewType CellStringValue::get_string() const { return value_; }

bool CellStringValue::equals(sptr<CellValue> other) const {
    return get_cmp(other) == 0;
}
bool CellStringValue::le(sptr<CellValue> other) const {
    return get_cmp(other) <= 0;
}
bool CellStringValue::lt(sptr<CellValue> other) const {
    return get_cmp(other) < 0;
}
bool CellStringValue::ge(sptr<CellValue> other) const {
    return get_cmp(other) >= 0;
}
bool CellStringValue::gt(sptr<CellValue> other) const {
    return get_cmp(other) > 0;
}
void CellStringValue::format(std::ostream& os) const {
    os << value_;
}

int CellStringValue::get_cmp(sptr<CellValue> other) const { 
    auto str_ptr = std::dynamic_pointer_cast<CellStringValue>(other);
    if(str_ptr == nullptr) 
	throw std::logic_error("Trying to compare CellStringValue with other type");
    size_t our_size = value_.size();
    size_t other_size = str_ptr->get_string().size();
    size_t mins = std::min(our_size, other_size);
    size_t maxs = std::max(our_size, other_size);
    int cmp = std::strncmp(value_.data(), str_ptr->get_string().data(), mins); 
    if(cmp != 0 || mins == maxs) return cmp;
    if (our_size == mins) return -1;
    return 1;
}

}
