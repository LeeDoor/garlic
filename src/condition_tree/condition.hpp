#pragma once
#include "expression.hpp"

namespace garlic {

template<typename ConditionResolver>
class Condition {
protected:
    using ExpressionPtr = std::unique_ptr<Expression<ConditionResolver>>;
public:
    virtual bool resolve() = 0;

protected:
    using ResolverPtr = std::shared_ptr<ConditionResolver>;

    Condition(std::shared_ptr<ConditionResolver> resolver)
    : resolver_{resolver} 
    {}

    std::shared_ptr<ConditionResolver> resolver_;
};

}

