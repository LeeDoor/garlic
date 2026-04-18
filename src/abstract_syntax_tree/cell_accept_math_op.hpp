#pragma once
#include "cell_value.hpp"
#include "expected_cell_value.hpp"

namespace garlic {

/// @ref CellValue subclass defining numbers or other entities
/// that accept math operations.
class CellAcceptMathOp : virtual public CellValue {
public:
    virtual ~CellAcceptMathOp() = default;

    virtual ExpectedCellValue add(sptr<CellValue> other) const = 0;
    virtual ExpectedCellValue sub(sptr<CellValue> other) const = 0;
    virtual ExpectedCellValue mul(sptr<CellValue> other) const = 0;
    virtual ExpectedCellValue div(sptr<CellValue> other) const = 0;
    virtual ExpectedCellValue remdiv(sptr<CellValue> other) const = 0;
    virtual ExpectedCellValue abs() const = 0;
    virtual ExpectedCellValue neg() const = 0;
};

}
