#pragma once
#include "query.hpp"
#include "expression.hpp"

namespace garlic {

class ExpressionSelectQuery : public Query {
public:
    ExpressionSelectQuery(sptr<Expression> expr);

    sptr<QueryResult> resolve(sptr<TableValueGatherer> gatherer) override;

private:
    sptr<Expression> expression_;
};

}
