#pragma once
#include "expression.hpp"

namespace garlic {

class Condition {
protected:
    using ExpressionPtr = std::unique_ptr<Expression>;
public:
    virtual bool resolve() = 0;

protected:
    using ResolverPtr = std::shared_ptr<ConditionResolver>;

    Condition(std::shared_ptr<ConditionResolver> resolver)
    : resolver_{resolver} 
    {}

    ResolverPtr resolver_;
};

}

