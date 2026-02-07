#pragma once
#include "cell_type.hpp"
#include "condition_resolver.hpp"

namespace garlic {

const std::string TYPE_MISMATCH = "calling get_<INT|FLOAT|STRING> to a function which does not match this data type.";
class CellValue {
protected:
    using CellValuePtr = std::shared_ptr<CellValue>;
    using ConditionResolverPtr = std::shared_ptr<ConditionResolver>;
public:
    virtual ~CellValue() = default;

    virtual bool equals(CellValuePtr other) = 0;
    virtual bool le(CellValuePtr other) = 0;
    virtual bool lt(CellValuePtr other) = 0;
    virtual bool ge(CellValuePtr other) = 0;
    virtual bool gt(CellValuePtr other) = 0;

    virtual StringViewType get_string() { throw std::logic_error(TYPE_MISMATCH); }
    virtual IntType get_int() { throw std::logic_error(TYPE_MISMATCH); }
    virtual FloatType get_float() { throw std::logic_error(TYPE_MISMATCH); }

    bool comparable_with(CellValuePtr other) {
        return cell_type_ == other->get_type();
    }
    CellType get_type() { return cell_type_; }

protected:
    CellValue(CellType cell_type) : cell_type_{ cell_type } {}
    CellType cell_type_;
};

}
