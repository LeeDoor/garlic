#include "math_expression.hpp"
#include "cell_accept_math_op.hpp"

namespace garlic {

sptr<CellValue> MathExpression::get_value(sptr<TableValueGatherer> gatherer) const {
    sptr<CellAcceptMathOp> 
	lhs = std::dynamic_pointer_cast<CellAcceptMathOp>(lhs_->get_value(gatherer)),
	rhs = std::dynamic_pointer_cast<CellAcceptMathOp>(rhs_->get_value(gatherer));
    if(!lhs || !rhs)
	throw std::logic_error("Invalid math operation on operands not allowing such actions");
    switch(op_) {
    case ADD:
	return lhs->add(rhs);
    case SUB:
	return lhs->sub(rhs);
    case DIV:
	return lhs->div(rhs);
    case MUL:
	return lhs->mul(rhs);
    case REMDIV:
	return lhs->remdiv(rhs);
    }
}

}
