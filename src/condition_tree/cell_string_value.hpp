#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellStringValue : public CellValue {
public:
    CellStringValue(StringViewType value)
    : CellValue(String)
    , value_{ value }
    {}

    StringViewType get_string() { return value_; }

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
        auto str_ptr = std::dynamic_pointer_cast<CellStringValue>(other);
        if(str_ptr == nullptr) 
            throw std::logic_error("Trying to compare CellStringValue with other type");
        size_t our_size = value_.size();
        size_t other_size = str_ptr->get_string().size();
        size_t mins = std::min(our_size, other_size);
        size_t maxs = std::max(our_size, other_size);
        int cmp = std::strncmp(value_.data(), str_ptr->get_string().data(), mins); 
        if(cmp != 0) {
            return cmp;
        }
        if(mins == maxs) return cmp;
        else if (our_size == mins) return -1;
        else return 1;
    }
protected:
    StringViewType value_;
};

}


