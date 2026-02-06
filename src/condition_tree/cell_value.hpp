#pragma once
#include "cell_type.hpp"
#include "condition_resolver.hpp"

namespace garlic {

const std::string TYPE_MISMATCH = "calling get_<INT|FLOAT|STRING> to a function which does not match this data type.";
enum BinaryOp { Equals, Gt, Ge, Ls, Le };
class CellValue {
protected:
    using ConditionResolverPtr = std::shared_ptr<ConditionResolver>;
    using CellValuePtr = std::shared_ptr<CellValue>;
public:
    virtual bool compare(CellValuePtr other, BinaryOp op) = 0;

    virtual StringViewType get_string() { throw std::logic_error(TYPE_MISMATCH); }
    virtual IntType get_int() { throw std::logic_error(TYPE_MISMATCH); }
    virtual FloatType get_float() { throw std::logic_error(TYPE_MISMATCH); }

    virtual bool compare_compatibility(CellValuePtr other) {
        return cell_type_ == other->get_type();
    }
    virtual CellType get_type() { return cell_type_; }

protected:
    CellType cell_type_;
};

}
