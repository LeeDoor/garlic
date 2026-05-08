#pragma once
#include "query_result.hpp"
#include "result_table.hpp"

namespace garlic {

class TableQueryResult : public QueryResult {
public:
    TableQueryResult(ResultTable&& table);

    StringViewType format() const override;

private:
    StringType table_result_;
};

class TableQueryResultGenerator {
public:
    static StringType form_table_result(ResultTable&& table) {
	replace_tabs(table);
	return TableQueryResultGenerator{ std::move(table) }.form_table_result();
    }

private:
    TableQueryResultGenerator(ResultTable&& table);

    static void replace_tabs(ResultTable& table);
    static std::vector<size_t> count_widths(const ResultTable& table);
    static std::unordered_map<size_t, size_t> count_heights(const ResultTable& table);

    StringType form_table_result();
    void print_table();
    void print_horizontal_delimeter();
    void print_table_header();
    void print_table_body();
    void print_row(size_t row_id, bool highlight_row = false);
    size_t get_height(size_t row);
    void highlight(StringViewType color, auto action);
    void print_row_subline(std::vector<size_t>& newline_pos, size_t row_id);
    std::tuple<StringViewType, bool> get_cell_subline(std::vector<size_t>& newline_pos, size_t row_id, size_t cell_id);
    void print_cell_subline(const StringViewType& cell_str, size_t column_id, bool is_last_subline);

    static constexpr char CROSS = '+';
    static constexpr char V_BAR = '|';
    static constexpr char H_BAR = '-';
    static constexpr char SPACE = ' ';
    static constexpr char ETC   = '+';

    std::vector<size_t> widths_;
    std::unordered_map<size_t, size_t> heights_;
    ResultTable table_;
    std::stringstream out_;
};

}
