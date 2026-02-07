#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellFloatValue : public CellValue {
public:
    CellFloatValue(FloatType value)
    : CellValue(Float)
    , value_{ value }
    {}

    FloatType get_float() { return value_; }

    bool equals(CellValuePtr other) override {
        return fl_equals(other);
    }
    bool le(CellValuePtr other) override {
        return fl_equals(other) || get_float() <= to_float(other);
    }
    bool lt(CellValuePtr other) override {
        return !fl_equals(other) && get_float() < to_float(other);
    }
    bool ge(CellValuePtr other) override {
        return fl_equals(other) || get_float() >= to_float(other);
    }
    bool gt(CellValuePtr other) override {
        return !fl_equals(other) && get_float() > to_float(other);
    }

private:
    FloatType to_float(CellValuePtr other) {
        auto float_ptr = std::dynamic_pointer_cast<CellFloatValue>(other);
        if(float_ptr == nullptr) 
            throw std::logic_error("Trying to compare float with other type");
        return float_ptr->get_float();
    }

    bool fl_equals(CellValuePtr other) {
        FloatType e = std::numeric_limits<FloatType>::epsilon();
        return std::abs(get_float() - to_float(other)) < e;
    }
protected:
    FloatType value_;
};

}

