#pragma once
#include "cell_type.hpp"
#include "query.hpp"
#include "expression.hpp"
#include "string_query_result.hpp"

namespace garlic {

using RequiredColumns = ArrayType<StringType>;

class ExpressionSelectQuery : public Query {
public:
    virtual ~ExpressionSelectQuery() = default;
    
    ExpressionSelectQuery(Expression::Ptr expr)
    : expression_ { std::move(expr) }
    {}

    QueryResult::Ptr resolve(TableValueGatherer::Ptr gatherer) override {
	auto result = expression_->get_value(gatherer);
	std::stringstream ss;
	result->format(ss);
	return std::make_unique<StringQueryResult>(ss.str());
    }

private:
    Expression::Ptr expression_;
};

}
