#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellStringValue : public CellValue {
public:
    CellStringValue(ConditionResolverPtr resolver, StringViewType value)
    : CellValue(resolver)
    , value_{ value }
    {}

    StringViewType get_string() override { return value_; }

    bool compare(CellValuePtr other, BinaryOp op) override {
        int comp = std::strncmp
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
    std::shared_ptr<ConditionResolver> resolver_;
    StringViewType value_;
};
   
}


