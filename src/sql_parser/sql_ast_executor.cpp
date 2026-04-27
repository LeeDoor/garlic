#include "sql_ast_executor.hpp"
#include "query_result.hpp"

namespace garlic::sql_parser {

SqlAstExecutor::SqlAstExecutor(ErrorPrinter& error_printer, const TableValueGathererFactory& gatherer_factory)
: os_{ std::cout }
, err_{ error_printer }
, gatherer_factory_{ gatherer_factory }
{}

void SqlAstExecutor::execute_sql_ast(const uptr<Query>& query) const {
    auto q_result = query->resolve(gatherer_factory_);
    if(q_result) {
	os_ << (*q_result)->format();
    } else {
	err_.print_error(q_result.error());
    }
}

}
