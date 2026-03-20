#pragma once
#include "cell_value.hpp"
#include "table_value_gatherer.hpp"

namespace garlic {

class Expression {
protected:
    using TableValueGathererPtr = std::shared_ptr<TableValueGatherer>;
    using CellValuePtr = std::shared_ptr<CellValue>;
public:
    using Ptr = std::unique_ptr<Expression>;
    virtual ~Expression() = default;

    virtual CellValuePtr get_value(TableValueGathererPtr gatherer) const = 0;
};

}
