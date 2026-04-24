#pragma once

#include "query_result.hpp"
namespace garlic {

class TableQueryResult : public QueryResult {
public:
    using Row = std::vector<StringType>;
    using Table = std::vector<Row>;
    TableQueryResult(Table&& table);

    StringViewType format() const override;
    
private:
    static StringType form_table_result(Table&& table);

    StringType table_result_;
};

}
