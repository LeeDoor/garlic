#pragma once
#include "expression.hpp"

namespace garlic {

class Condition {
protected:
    using ExpressionPtr = std::unique_ptr<Expression>;
public:
    virtual bool resolve() = 0;

protected:
    Condition() {}
};

}

