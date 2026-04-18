#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellErrorValue : public CellValue {
public:
    CellErrorValue(const StringType& error);

    void format(std::ostream& os) const override;
private:
    StringType error_message_;
};

}
