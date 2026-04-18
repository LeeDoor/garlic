#include "cell_error_value.hpp"

namespace garlic {

CellErrorValue::CellErrorValue(const StringType& error)
: CellValue{ Error }
, error_message_{ std::move(error) }
{}

void CellErrorValue::format(std::ostream& os) const {
    os << error_message_ << std::endl;
}

}
