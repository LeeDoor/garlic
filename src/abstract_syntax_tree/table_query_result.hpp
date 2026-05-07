#pragma once
#include "query_result.hpp"
#include "result_table.hpp"

namespace garlic {

class TableQueryResult : public QueryResult {
public:
    TableQueryResult(ResultTable&& table);

    StringViewType format() const override;
    
private:
    static StringType form_table_result(ResultTable&& table);
    static void print_table(std::ostream& out, const ResultTable& table);
    static void print_horizontal_delimeter(std::ostream& out, const std::vector<size_t>& widths);
    static std::vector<size_t> count_width_foreach_column(const ResultTable& table);
    static void print_table_header(std::ostream& out, const ResultRow& header, const std::vector<size_t>& widths);
    static void print_table_body(std::ostream& out, const ResultTable& table, const std::vector<size_t>& widths);

    static constexpr char CROSS = '+';
    static constexpr char V_BAR = '|';
    static constexpr char H_BAR = '-';
    static constexpr char SPACE = ' ';
    static constexpr char ETC   = '+';

    StringType table_result_;
};

}
