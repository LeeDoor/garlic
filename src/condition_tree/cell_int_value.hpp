#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellIntValue : public CellValue {
public:
    CellIntValue(IntType value)
    : value_{ value }
    {}

    IntType get_int() override { return value_; }
    bool compare(CellValuePtr other, BinaryOperation op) override {
        switch(op) {
        case Equals:
            return get_int() == other->get_int();
        case Gt:
            return get_int() > other->get_int();
        case Ge:
            return get_int() >= other->get_int();
        case Ls:
            return get_int() < other->get_int();
        case Le:
            return get_int() <= other->get_int();
        default:
            throw std::logic_error("Not implemented binary operator in CellIntValue");
        };
    }

protected:
    IntType value_;
};
   
}
