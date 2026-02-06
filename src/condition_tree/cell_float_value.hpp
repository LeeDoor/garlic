#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellFloatValue : public CellValue {
public:
    CellFloatValue(FloatType value)
    : value_{ value }
    {}

    FloatType get_float() override { return value_; }

    bool compare(CellValuePtr other, BinaryOp op) override {
        FloatType e = std::numeric_limits<FloatType>::epsilon();
        switch(op) {
        case Equals:
            return std::abs(get_float() - other->get_float()) < e;
        case Gt:
            return get_float() > other->get_float();
        case Ge:
            return get_float() >= other->get_float();
        case Ls:
            return get_float() < other->get_float();
        case Le:
            return get_float() <= other->get_float();
          break;
        };
    }

protected:
    FloatType value_;
};
   
}

