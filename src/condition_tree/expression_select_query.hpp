#pragma once
#include "cell_type.hpp"
#include "query.hpp"
#include "expression.hpp"

namespace garlic {

using RequiredColumns = ArrayType<StringType>;

class ExpressionSelectQuery : public Query {
public:
    ExpressionSelectQuery(sptr<Expression> expr);

    sptr<QueryResult> resolve(sptr<TableValueGatherer> gatherer) override;

private:
    sptr<Expression> expression_;
};

}
