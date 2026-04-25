#pragma once
#include "expression.hpp"

namespace garlic {

enum BinaryMathOperator { Add, Sub, Div, Mul, Remdiv };
class CellAcceptMathOp;

class BinaryMathExpression : public Expression {
public:
    BinaryMathExpression(sptr<Expression> lhs, sptr<Expression> rhs, BinaryMathOperator op);

    ExpectedCellValue resolve(sptr<CellValueGatherer> gatherer) const override;
private:
    sptr<Expression> lhs_, rhs_;
    BinaryMathOperator op_;
};

}
