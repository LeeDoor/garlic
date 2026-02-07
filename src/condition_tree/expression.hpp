#pragma once
#include "cell_type.hpp"
#include "cell_value.hpp"
#include "condition_resolver.hpp"

namespace garlic {

class Expression {
protected:
    using CellValuePtr = std::shared_ptr<CellValue>;
    using ResolverPtr = std::shared_ptr<ConditionResolver>;
public:
    virtual ~Expression() = default;

    virtual CellValuePtr get_value() const = 0;

    virtual CellType get_type() const {
        return cell_type_;
    }
protected:
    Expression(CellType type, ResolverPtr resolver)
    : cell_type_{ type }
    , resolver_{ resolver } 
    {}

    CellType cell_type_;
    ResolverPtr resolver_;
};

}
