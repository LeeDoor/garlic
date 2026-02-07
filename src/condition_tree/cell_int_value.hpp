#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellIntValue : public CellValue {
public:
    CellIntValue(IntType value)
    : CellValue(Int)
    , value_{ value }
    {}

    IntType get_int() override { return value_; }

    bool equals(CellValuePtr other) override {
        return get_int() == other->get_int();
    }
    bool le(CellValuePtr other) override {
        return get_int() <= other->get_int();
    }
    bool lt(CellValuePtr other) override {
        return get_int() < other->get_int();
    }
    bool ge(CellValuePtr other) override {
        return get_int() >= other->get_int();
    }
    bool gt(CellValuePtr other) override {
        return get_int() > other->get_int();
    }

protected:
    IntType value_;
};


}
