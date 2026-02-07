#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellStringValue : public CellValue {
public:
    CellStringValue(StringViewType value)
    : CellValue(String)
    , value_{ value }
    {}

    StringViewType get_string() override { return value_; }

    bool equals(CellValuePtr other) override {
        return get_cmp(other) == 0;
	}
    bool le(CellValuePtr other) override {
        return get_cmp(other) <= 0;
	}
    bool lt(CellValuePtr other) override {
        return get_cmp(other) < 0;
	}
    bool ge(CellValuePtr other) override {
        return get_cmp(other) >= 0;
	}
    bool gt(CellValuePtr other) override {
        return get_cmp(other) > 0;
	}

private:
    int get_cmp(CellValuePtr other) { 
        return std::strncmp(value_.data(), other->get_string().data(), value_.size()); 
    }
protected:
    StringViewType value_;
};
   
}


