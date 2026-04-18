#include "unary_math_expression.hpp"
#include "cell_accept_math_op.hpp"

namespace garlic {

UnaryMathExpression::UnaryMathExpression(sptr<Expression> operand, UnaryMathOperator op)
: Expression{ TypeRules::unary_math_comp(operand->get_type()) }
, operand_{ operand }
, op_{ op }
{}

std::optional<StringType> UnaryMathExpression::validate() const { 
    return CanBeValidated::validate(operand_->get_type()); 
}
UnaryMathExpression::ExpectedCellValue UnaryMathExpression::get_value(sptr<TableValueGatherer> gatherer) const {
    const auto value = operand_->get_value(gatherer); if(!value) return value;
    sptr<CellAcceptMathOp> operand = std::dynamic_pointer_cast<CellAcceptMathOp>(*value);
    if(!operand)
	throw std::logic_error("Invalid math operation on operands not allowing such actions");
    switch(op_) {
    case Abs:
	return operand->abs();
    case Neg:
	return operand->neg();
      break;
    }
    throw std::logic_error("Unary math operator not implemented in unary math expression");
}

}
