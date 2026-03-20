#pragma once
#include "cell_value.hpp"
#include "table_value_gatherer.hpp"

namespace garlic {

class Expression {
public:
    virtual ~Expression() = default;

    virtual sptr<CellValue> get_value(sptr<TableValueGatherer> gatherer) const = 0;
};

}
