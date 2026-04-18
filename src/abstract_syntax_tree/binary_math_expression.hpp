#pragma once
#include "expression.hpp"

namespace garlic {

enum BinaryMathOperator { Add, Sub, Div, Mul, Remdiv };
class CellAcceptMathOp;

class BinaryMathExpression : public Expression {
public:
    BinaryMathExpression(sptr<Expression> lhs, sptr<Expression> rhs, BinaryMathOperator op);

    std::optional<StringType> validate() const override;
    ExpectedCellValue get_value(sptr<TableValueGatherer> gatherer) const override;
private:
    static std::expected<sptr<CellAcceptMathOp>, ErrorType> to_cell_accepting_math(ExpectedCellValue cell_value);

    sptr<Expression> lhs_, rhs_;
    BinaryMathOperator op_;
};

}
