#pragma once
#include "cell_type.hpp"
#include "cell_value.hpp"

namespace garlic {

class Expression {
protected:
    using CellValuePtr = std::shared_ptr<CellValue>;
public:
    virtual ~Expression() = default;

    virtual CellValuePtr get_value() const = 0;

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
