#pragma once
#include "error_printer.hpp"
#include "query.hpp"
#include "table_value_gatherer_factory.hpp"

namespace garlic::sql_parser {

/// Executes an AST and prints result to std::cout.
class SqlAstExecutor {
public:
    SqlAstExecutor(ErrorPrinter& error_printer, const TableValueGathererFactory& gatherer_factory);

    void execute_sql_ast(const uptr<Query>& query) const;
    
private:
    std::ostream& os_;
    ErrorPrinter& err_;
    const TableValueGathererFactory& gatherer_factory_;
};

}
