#pragma once
#include "cell_accept_math_op.hpp"
#include "cell_type.hpp"

namespace garlic {

class CellFloatValue;

class CellIntValue : public CellAcceptMathOp {
public:
    CellIntValue(IntType value);

    explicit operator IntType() const;
    IntType get_int() const;
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

protected:
    IntType value_;
};

}
