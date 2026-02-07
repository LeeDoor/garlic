#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellFloatValue : public CellValue {
public:
    CellFloatValue(FloatType value)
    : CellValue(Float)
    , value_{ value }
    {}

    FloatType get_float() override { return value_; }

    bool equals(CellValuePtr other) override {
        return fl_equals(other);
    }
    bool le(CellValuePtr other) override {
        return fl_equals(other) && get_float() <= other->get_float();
    }
    bool lt(CellValuePtr other) override {
        return !fl_equals(other) && get_float() < other->get_float();
    }
    bool ge(CellValuePtr other) override {
        return fl_equals(other) && get_float() >= other->get_float();
    }
    bool gt(CellValuePtr other) override {
        return !fl_equals(other) && get_float() > other->get_float();
    }

private:
    bool fl_equals(CellValuePtr other) {
        FloatType e = std::numeric_limits<FloatType>::epsilon();
        return std::abs(get_float() - other->get_float()) < e;
    }
protected:
    FloatType value_;
};

}

