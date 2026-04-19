#include "binary_math_expression.hpp"
#include "cell_accept_math_op.hpp"

namespace garlic {

BinaryMathExpression::BinaryMathExpression(sptr<Expression> lhs, sptr<Expression> rhs, BinaryMathOperator op)
: Expression{ TypeRules::binary_math_comp(lhs->get_type(), rhs->get_type()) }
, lhs_{ lhs }
, rhs_{ rhs }
, op_ { op }
{}

ExpectedValid BinaryMathExpression::validate() const {
    return CanBeValidated::validate(lhs_->get_type(), rhs_->get_type());
}
ExpectedCellValue BinaryMathExpression::resolve(sptr<TableValueGatherer> gatherer) const {
    const auto lvalue = lhs_->resolve(gatherer); if(!lvalue) return lvalue;
    const auto rvalue = rhs_->resolve(gatherer); if(!rvalue) return rvalue;
    sptr<CellAcceptMathOp> lhs = std::dynamic_pointer_cast<CellAcceptMathOp>(*lvalue);
    sptr<CellAcceptMathOp> rhs = std::dynamic_pointer_cast<CellAcceptMathOp>(*rvalue);
    if(!lhs || !rhs)
	throw std::logic_error("Invalid math operation on operands not allowing such actions"); 

    switch(op_) {
    case Add:
	return lhs->add(rhs);
    case Sub:
	return lhs->sub(rhs);
    case Div:
	return lhs->div(rhs);
    case Mul:
	return lhs->mul(rhs);	
    case Remdiv:
	return lhs->remdiv(rhs);
    }
    throw std::logic_error("Unary math operator not implemented in unary math expression");
}

}
