#pragma once
#include "cell_type.hpp"
#include "condition_resolver.hpp"

namespace garlic {

template<IConditionResolver ConditionResolver> 
class Expression {
public:
    virtual StringViewType get_string() = 0;
    virtual IntType get_int() = 0;
    virtual FloatType get_float() = 0;

    virtual CellType get_type() {
        return cell_type_;
    }
protected:
    using ResolverPtr = std::shared_ptr<ConditionResolver>;

    Expression(std::shared_ptr<ConditionResolver> resolver)
    : resolver_{resolver} 
    {}

    std::shared_ptr<ConditionResolver> resolver_;
    CellType cell_type_;
};

}
