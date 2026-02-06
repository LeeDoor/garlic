#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellStringValue : public CellValue {
public:
    CellStringValue(StringViewType value)
    : value_{ value }
    {}

    StringViewType get_string() override { return value_; }

    bool compare(CellValuePtr other, BinaryOperation op) override {
        int comp = std::strncmp(value_.data(), other->get_string().data(), value_.size());
        switch(op) {
        case Equals:
            return comp == 0;
        case Gt:
            return comp > 0;
        case Ge:
            return comp >= 0;
        case Ls:
            return comp < 0;
        case Le:
            return comp <= 0;
          break;
        };
    }

protected:
    StringViewType value_;
};
   
}


