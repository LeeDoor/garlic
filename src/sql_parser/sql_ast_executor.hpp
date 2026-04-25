#pragma once
#include "error_printer.hpp"
#include "query.hpp"

namespace garlic::sql_parser {

/// Executes an AST and prints result to std::cout.
class SqlAstExecutor {
public:
    SqlAstExecutor(ErrorPrinter& error_printer, sptr<TableValueGatherer> gatherer);

    void execute_sql_ast(const uptr<Query>& query) const;
    
private:
    std::ostream& os_;
    ErrorPrinter& err_;
    sptr<TableValueGatherer> gatherer_;
};

}
