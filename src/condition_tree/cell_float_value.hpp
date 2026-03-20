#pragma once
#include "cell_accept_math_op.hpp"

namespace garlic {
class CellIntValue;
class CellFloatValue : public CellAcceptMathOp {
public:
    CellFloatValue(FloatType value);
    FloatType get_float() const;

    bool equals(CellValuePtr other) const override;
    bool le(CellValuePtr other) const override;
    bool lt(CellValuePtr other) const override;
    bool ge(CellValuePtr other) const override;
    bool gt(CellValuePtr other) const override;
    void format(std::ostream& os) const override;

    CellValuePtr add(CellValuePtr other) const override;
    CellValuePtr sub(CellValuePtr other) const override;
    CellValuePtr mul(CellValuePtr other) const override;
    CellValuePtr div(CellValuePtr other) const override;

private:
    FloatType to_float(CellValuePtr other) const;

    bool fl_equals(CellValuePtr other) const;
protected:
    FloatType value_;
};

}

