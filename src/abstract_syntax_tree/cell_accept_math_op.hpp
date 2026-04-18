#pragma once
#include "cell_value.hpp"

namespace garlic {

/// @ref CellValue subclass defining numbers or other entities
/// that accept math operations.
class CellAcceptMathOp : virtual public CellValue {
public:
    virtual ~CellAcceptMathOp() = default;

    virtual sptr<CellValue> add(sptr<CellValue> other) const = 0;
    virtual sptr<CellValue> sub(sptr<CellValue> other) const = 0;
    virtual sptr<CellValue> mul(sptr<CellValue> other) const = 0;
    virtual sptr<CellValue> div(sptr<CellValue> other) const = 0;
    virtual sptr<CellValue> remdiv(sptr<CellValue> other) const = 0;
    virtual sptr<CellValue> abs() const = 0;
    virtual sptr<CellValue> neg() const = 0;
};

}
