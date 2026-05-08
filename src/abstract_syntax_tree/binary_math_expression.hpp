#pragma once
#include "expression.hpp"

namespace garlic {

enum BinaryMathOperator { Add, Sub, Div, Mul, Remdiv };
class CellAcceptMathOp;

class BinaryMathExpression : public Expression {
public:
    BinaryMathExpression(sptr<Expression> lhs, sptr<Expression> rhs, BinaryMathOperator op);

    ExpectedCellValue resolve(const TablesGathered& gatherers) const override;
    UsedTables get_used_tables() const override;

private:
    sptr<Expression> lhs_, rhs_;
    BinaryMathOperator op_;
};

}
