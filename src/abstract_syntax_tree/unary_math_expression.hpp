#pragma once
#include "expression.hpp"

namespace garlic {

enum UnaryMathOperator { Abs, Neg };

class UnaryMathExpression : public Expression {
public:
    UnaryMathExpression(sptr<Expression> operand, UnaryMathOperator op);

    ExpectedCellValue resolve(sptr<CellValueGatherer> gatherer) const override;

private:
    sptr<Expression> operand_;
    UnaryMathOperator op_;
};

}
