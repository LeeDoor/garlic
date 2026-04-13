#pragma once
#include "query.hpp"

namespace garlic::sql_parser {

/// Executes an AST and prints result to std::cout.
class SqlAstExecutor {
public:
    SqlAstExecutor();

    void execute_sql_ast(const uptr<Query>& query) const;
    
private:
    std::ostream& os_;
    sptr<TableValueGatherer> gatherer_;
};

}
