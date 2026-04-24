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
    static void print_table(std::ostream& out, const Table& table);
    static void print_horizontal_delimeter(std::ostream& out, const std::vector<size_t>& widths);
    static std::vector<size_t> count_width_foreach_column(const Table& table);

    static constexpr char CROSS = '+';
    static constexpr char V_BAR = '|';
    static constexpr char H_BAR = '-';
    static constexpr char SPACE = ' ';

    StringType table_result_;
};

}
