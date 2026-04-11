#pragma once
#include "query.hpp"

namespace garlic::sql_parser {

class SqlAstExecutor {
public:
    SqlAstExecutor();

    void execute_sql_ast(const uptr<Query>& query) const;
    
private:
    std::ostream& os_;
    sptr<TableValueGatherer> gatherer_;
};

}
