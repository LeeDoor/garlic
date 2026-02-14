#pragma once
#include "cell_value.hpp"
#include "table_value_gatherer.hpp"

namespace garlic {

class Expression {
protected:
    using CellValuePtr = std::shared_ptr<CellValue>;
    using TableValueGathererPtr = std::shared_ptr<TableValueGatherer>;
public:
    virtual ~Expression() = default;

    virtual CellValuePtr get_value(TableValueGathererPtr gatherer) const = 0;
};

}
