#include "cell_string_view_value.hpp"

namespace garlic {

CellStringViewValue::CellStringViewValue(StringViewType value)
    : CellValue(String)
      , value_{ value }
{}

CellStringViewValue::operator StringViewType() const {
    return value_;
}

StringViewType CellStringViewValue::get_string_view() const { return value_; }

bool CellStringViewValue::equals(sptr<CellValue> other) const {
    return get_cmp(other) == 0;
}
bool CellStringViewValue::le(sptr<CellValue> other) const {
    return get_cmp(other) <= 0;
}
bool CellStringViewValue::lt(sptr<CellValue> other) const {
    return get_cmp(other) < 0;
}
bool CellStringViewValue::ge(sptr<CellValue> other) const {
    return get_cmp(other) >= 0;
}
bool CellStringViewValue::gt(sptr<CellValue> other) const {
    return get_cmp(other) > 0;
}
void CellStringViewValue::format(std::ostream& os) const {
    os << value_;
}

int CellStringViewValue::get_cmp(sptr<CellValue> other) const { 
    auto str_ptr = std::dynamic_pointer_cast<CellStringViewValue>(other);
    if(str_ptr == nullptr) 
	throw std::logic_error("Trying to compare CellStringViewValue with other type");
    size_t our_size = value_.size();
    size_t other_size = str_ptr->get_string_view().size();
    size_t mins = std::min(our_size, other_size);
    size_t maxs = std::max(our_size, other_size);
    int cmp = std::strncmp(value_.data(), str_ptr->get_string_view().data(), mins); 
    if(cmp != 0 || mins == maxs) return cmp;
    if (our_size == mins) return -1;
    return 1;
}

}
