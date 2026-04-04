#pragma once
#include "query.hpp"

namespace garlic::sql_parser {

class SqlAstExecutor {
public:
    SqlAstExecutor();

    void print_sql_ast(uptr<Query> query) const;
    
private:
    std::ostream& os_;
    sptr<TableValueGatherer> gatherer_;
};

}
