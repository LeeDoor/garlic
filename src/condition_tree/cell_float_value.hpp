#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellFloatValue : public CellValue {
public:
    CellFloatValue(FloatType value)
    : value_{ value }
    {}

    FloatType get_float() override { return value_; }

    bool compare(CellValuePtr other, BinaryOperation op) override {
        auto equals = [this, other] {
            FloatType e = std::numeric_limits<FloatType>::epsilon();
            return std::abs(get_float() - other->get_float()) < e;
        };
        switch(op) {
        case Equals:
            return equals();
        case Gt:
            return !equals() && get_float() > other->get_float();
        case Ge:
            return equals() && get_float() >= other->get_float();
        case Ls:
            return !equals() && get_float() < other->get_float();
        case Le:
            return equals() && get_float() <= other->get_float();
          break;
        };
    }

protected:
    FloatType value_;
};
   
}

