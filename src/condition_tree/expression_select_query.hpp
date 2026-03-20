#pragma once
#include "cell_type.hpp"
#include "query.hpp"
#include "expression.hpp"

namespace garlic {

using RequiredColumns = ArrayType<StringType>;

class ExpressionSelectQuery : public Query {
public:
    ExpressionSelectQuery(Expression::Ptr expr);

    QueryResult::Ptr resolve(TableValueGatherer::Ptr gatherer) override;

private:
    Expression::Ptr expression_;
};

}
