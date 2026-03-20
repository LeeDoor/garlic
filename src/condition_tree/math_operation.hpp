#pragma once
#include "expression.hpp"

namespace garlic {

enum MathOperator { ADD, SUB, DIV, MUL, REMDIV };

class MathOperation : public Expression {
public:
    sptr<CellValue> get_value(sptr<TableValueGatherer> gatherer) const override {
        sptr<CellValue> lhs = lhs_->get_value(gatherer),
                     rhs = rhs_->get_value(gatherer);
        switch(op_) {
        case ADD:
        return lhs 
        case SUB:
        case DIV:
        case MUL:
        case REMDIV:
          break;
        }
    }

private:
    MathOperator op_;
    sptr<Expression> lhs_, rhs_;
};

}
