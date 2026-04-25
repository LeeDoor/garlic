#include "sql_ast_executor.hpp"
#include "query_result.hpp"

namespace garlic::sql_parser {

SqlAstExecutor::SqlAstExecutor(ErrorPrinter& error_printer, sptr<TableValueGatherer> gatherer)
: os_{ std::cout }
, err_{ error_printer }
, gatherer_{ gatherer }
{}

void SqlAstExecutor::execute_sql_ast(const uptr<Query>& query) const {
    // foreach table in tables
    // if constrain()
    auto q_result = query->resolve(gatherer_);
    if(q_result) {
	os_ << (*q_result)->format();
    } else {
	err_.print_error(q_result.error());
    }
}

}
