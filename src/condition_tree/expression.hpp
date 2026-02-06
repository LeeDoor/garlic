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
    virtual CellValuePtr get_value() = 0;

    virtual CellType get_type() {
        return cell_type_;
    }
protected:
    Expression(std::shared_ptr<ConditionResolver> resolver)
    : resolver_{resolver} 
    {}

    std::shared_ptr<ConditionResolver> resolver_;
    CellType cell_type_;
};

}
