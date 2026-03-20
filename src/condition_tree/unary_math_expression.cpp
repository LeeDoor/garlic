#include "unary_math_expression.hpp"
#include "cell_accept_math_op.hpp"

namespace garlic {

UnaryMathExpression::UnaryMathExpression(sptr<Expression> operand, UnaryMathOperator op)
: operand_{ operand }
, op_{ op }
{}

sptr<CellValue> UnaryMathExpression::get_value(sptr<TableValueGatherer> gatherer) const {
    sptr<CellAcceptMathOp> 
	operand = std::dynamic_pointer_cast<CellAcceptMathOp>(operand_->get_value(gatherer));
    if(!operand)
	throw std::logic_error("Invalid math operation on operands not allowing such actions");
    switch(op_) {
    case ABS:
	return operand->abs();
    case NEG:
	return operand->neg();
      break;
    }
    throw std::logic_error("Unary math operator not implemented in unary math expression");
}

}
