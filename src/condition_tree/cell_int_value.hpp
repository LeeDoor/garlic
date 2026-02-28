#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellIntValue : public CellValue {
public:
    CellIntValue(IntType value)
    : CellValue(Int)
    , value_{ value }
    {}

    IntType get_int() const { return value_; }

    bool equals(CellValuePtr other) const override {
        return get_int() == to_int(other);
    }
    bool le(CellValuePtr other) const override {
        return get_int() <= to_int(other);
    }
    bool lt(CellValuePtr other) const override {
        return get_int() < to_int(other);
    }
    bool ge(CellValuePtr other) const override {
        return get_int() >= to_int(other);
    }
    bool gt(CellValuePtr other) const override {
        return get_int() > to_int(other);
    }

private:
    IntType to_int(CellValuePtr other) const {
        auto int_ptr = std::dynamic_pointer_cast<CellIntValue>(other);
        if(int_ptr == nullptr) throw std::logic_error("Comparing CellIntValue with other type");
        return int_ptr->get_int();
    }

protected:
    IntType value_;
};

}
