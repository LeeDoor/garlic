#pragma once
#include "cell_string_view_value.hpp"

namespace garlic {

class CellStringValue : public CellStringViewValue {
public:
    CellStringValue(StringType&& value)
    : CellValue{ String }
    , CellStringViewValue{ "" }
    , stored_{ std::move(value) }
    {
	CellStringViewValue::value_ = stored_;
    }

protected:
    StringType stored_;
};

}
