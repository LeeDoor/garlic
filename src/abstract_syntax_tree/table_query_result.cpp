#include "table_query_result.hpp"

namespace garlic {

TableQueryResult::TableQueryResult(Table&& table) 
: table_result_{ form_table_result(std::move(table)) }
{}

StringType TableQueryResult::form_table_result(Table&& table) {
    std::stringstream ss;
    print_table(ss, table);
    return ss.str();
}

StringViewType TableQueryResult::format() const {
    return table_result_;
}

void TableQueryResult::print_table(std::ostream& out, const Table& table) {
    if (table.empty()) { return; }
    auto widths = count_width_foreach_column(table);
    print_horizontal_delimeter(out, widths);
    for (std::size_t r = 0; r < table.size(); ++r) {
        out << V_BAR;
        for (std::size_t c = 0; c < table[0].size(); ++c) {
            out << SPACE
                << std::left
                << std::setw(static_cast<int>(widths[c]))
                << table[r][c]
                << SPACE
                << V_BAR;
        }
        out << std::endl;
        if (r == 0 && table.size() > 1) {
            print_horizontal_delimeter(out, widths);
        }
    }

    print_horizontal_delimeter(out, widths);
}
std::vector<size_t> TableQueryResult::count_width_foreach_column(const Table& table) {
    const std::size_t cols = table[0].size();
    std::vector<std::size_t> widths(cols, 0);
    for (const auto& row : table) {
        for (std::size_t i = 0; i < cols; ++i) {
            widths[i] = std::max(widths[i], row[i].size());
        }
    }
    return widths;
}

void TableQueryResult::print_horizontal_delimeter(std::ostream& out, const std::vector<std::size_t>& widths) {
    out << CROSS;
    for (std::size_t i = 0; i < widths.size(); ++i) {
        out << std::string(widths[i] + 2, H_BAR)
	    << CROSS;
    }
    out << '\n';
}


}
