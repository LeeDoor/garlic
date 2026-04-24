#include "sql_ast_executor.hpp"
#include "dumb_table_value_gatherer.hpp"
#include "query_result.hpp"

namespace garlic::sql_parser {

SqlAstExecutor::SqlAstExecutor()
: os_{ std::cout }
, gatherer_{ std::make_unique<DumbTableValueGatherer>() }
{}

void SqlAstExecutor::execute_sql_ast(const uptr<Query>& query) const {
    sptr<QueryResult> q_result = query->resolve(gatherer_);
    os_ << q_result->format();
}

}
