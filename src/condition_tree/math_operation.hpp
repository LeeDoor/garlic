#pragma once
#include "expression.hpp"

namespace garlic {

enum MathOperator { ADD, SUB, DIV, MUL, REMDIV };

class MathOperation : public Expression {
public:
    CellValuePtr get_value(TableValueGathererPtr gatherer) const override {
        CellValuePtr lhs = lhs_->get_value(gatherer),
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
    Expression::Ptr lhs_, rhs_;
};

}
