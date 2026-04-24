#include "sql_ast_executor.hpp"
#include "dumb_table_value_gatherer.hpp"
#include "query_result.hpp"

namespace garlic::sql_parser {

SqlAstExecutor::SqlAstExecutor(ErrorPrinter& error_printer)
: os_{ std::cout }
, err_{ error_printer }
, gatherer_{ std::make_unique<DumbTableValueGatherer>() }
{}

void SqlAstExecutor::execute_sql_ast(const uptr<Query>& query) const {
    auto q_result = query->resolve(gatherer_);
    if(q_result) {
	os_ << (*q_result)->format();
    } else {
	err_.print_error(q_result.error());
    }
}

}
