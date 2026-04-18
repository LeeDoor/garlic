#pragma once
#include "cell_accept_math_op.hpp"
#include "cell_comparable.hpp"

namespace garlic {
class CellIntValue;
class CellFloatValue : public CellComparable, public CellAcceptMathOp {
public:
    CellFloatValue(FloatType value);

    explicit operator FloatType() const;
    FloatType get_float() const;

    bool equals(sptr<CellValue> other) const override;
    bool le(sptr<CellValue> other) const override;
    bool lt(sptr<CellValue> other) const override;
    bool ge(sptr<CellValue> other) const override;
    bool gt(sptr<CellValue> other) const override;
    void format(std::ostream& os) const override;

    ExpectedCellValue add(sptr<CellValue> other) const override;
    ExpectedCellValue sub(sptr<CellValue> other) const override;
    ExpectedCellValue mul(sptr<CellValue> other) const override;
    ExpectedCellValue div(sptr<CellValue> other) const override;
    ExpectedCellValue remdiv(sptr<CellValue> other) const override;
    ExpectedCellValue abs() const override;
    ExpectedCellValue neg() const override;

private:
    static FloatType number_to_float(sptr<CellValue> other);
    bool fl_equals(sptr<CellValue> other) const;

protected:
    FloatType value_;
};

}

