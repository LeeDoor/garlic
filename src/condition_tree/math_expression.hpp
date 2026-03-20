#pragma once
#include "expression.hpp"

namespace garlic {

enum MathOperator { ADD, SUB, DIV, MUL, REMDIV };
class CellAcceptMathOp;

class MathExpression : public Expression {
public:
    sptr<CellValue> get_value(sptr<TableValueGatherer> gatherer) const override;

private:
    MathOperator op_;
    sptr<Expression> lhs_, rhs_;
};

}
