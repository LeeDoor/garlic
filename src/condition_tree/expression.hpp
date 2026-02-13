#pragma once
#include "cell_type.hpp"
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

    virtual CellType get_type() const {
        return cell_type_;
    }

protected:
    Expression(CellType type)
    : cell_type_{ type }
    {}

    CellType cell_type_;
};

}
