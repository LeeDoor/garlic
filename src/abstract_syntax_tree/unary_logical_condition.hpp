#pragma once
#include "condition.hpp"
#include "logical_operator.hpp"

namespace garlic {

class UnaryLogicalCondition : public Condition {
public:
    UnaryLogicalCondition(sptr<Condition> condition, UnaryLogicalOperator op);

    ExpectedBoolean resolve(sptr<TableValueGatherer> gatherer) const override;
    std::optional<StringType> validate() const override { return std::nullopt; }

protected:
    sptr<Condition> cond_;
    UnaryLogicalOperator op_;
};

}
