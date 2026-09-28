#pragma once
#include "binary_operator.hpp"
#include "condition.hpp"

namespace garlic::abstract_syntax_tree {
class Expression;

class CompareCondition : public Condition {
  public:
	CompareCondition(sptr<Expression> lhs, sptr<Expression> rhs, BinaryOperator op);

	ExpectedCellBooleanValue resolve_bool(const TablesGathered &gatherers) const override;
	UsedTables get_used_tables() const override;

  private:
	sptr<Expression> lhs_;
	sptr<Expression> rhs_;
	BinaryOperator operator_;
};

} // namespace garlic::abstract_syntax_tree
