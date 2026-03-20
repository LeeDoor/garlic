#pragma once
#include "cell_accept_math_op.hpp"

namespace garlic {

class CellFloatValue;

class CellIntValue : public CellAcceptMathOp {
public:
    CellIntValue(IntType value);

    explicit operator IntType() const;
    IntType get_int() const;
    FloatType get_float() const;

    bool equals(sptr<CellValue> other) const override;
    bool le(sptr<CellValue> other) const override;
    bool lt(sptr<CellValue> other) const override;
    bool ge(sptr<CellValue> other) const override;
    bool gt(sptr<CellValue> other) const override;
    void format(std::ostream& os) const override;

    sptr<CellValue> add(sptr<CellValue> other) const override;
    sptr<CellValue> sub(sptr<CellValue> other) const override;
    sptr<CellValue> mul(sptr<CellValue> other) const override;
    sptr<CellValue> div(sptr<CellValue> other) const override;
    sptr<CellValue> remdiv(sptr<CellValue> other) const override;

protected:
    IntType value_;
};

}
