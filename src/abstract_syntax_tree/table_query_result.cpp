#include "table_query_result.hpp"

namespace garlic {

TableQueryResult::TableQueryResult(ResultTable&& table) 
: table_result_{ form_table_result(std::move(table)) }
{}

StringType TableQueryResult::form_table_result(ResultTable&& table) {
    std::stringstream ss;
    print_table(ss, table);
    return ss.str();
}

StringViewType TableQueryResult::format() const {
    return table_result_;
}

void TableQueryResult::print_table(std::ostream& out, const ResultTable& table) {
    if (table.empty()) { return; }
    auto widths = count_width_foreach_column(table);
    print_horizontal_delimeter(out, widths);
    print_table_header(out, table.front(), widths);
    print_horizontal_delimeter(out, widths);
    print_table_body(out, table, widths);
    if(table.size() > 1)
	print_horizontal_delimeter(out, widths);
}

std::vector<size_t> TableQueryResult::count_width_foreach_column(const ResultTable& table) {
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

void TableQueryResult::print_table_header(std::ostream& out, const ResultRow& header, const std::vector<size_t>& widths) {
    out << V_BAR;
    for (std::size_t c = 0; c < header.size(); ++c) {
	auto newline_delimeter = header[c].find('\n');
	auto first_line = header[c].substr(0, newline_delimeter);
	out << SPACE
	    << std::left
	    << std::setw(static_cast<int>(widths[c]))
	    << first_line
	    << SPACE
	    << V_BAR;
    }
    out << std::endl;
}

std::unordered_map<size_t, size_t> count_heights(const ResultTable& table) {
    std::unordered_map<size_t, size_t> result;
    for(size_t r = 0; r < table.size(); ++r) {
	auto& row = table[r];
	size_t row_height = 1;
	for(size_t c = 0; c < row.size(); ++c) {
	    auto& cell = row[c];
	    size_t lines = 1 + std::count(cell.begin(), cell.end(), '\n');
	    row_height = std::max(row_height, lines);
	}
	if(row_height > 1)
	    result[r] = row_height;
    }
    return result;
}

void TableQueryResult::print_table_body(std::ostream& out, const ResultTable& table, const std::vector<size_t>& widths) {
    std::unordered_map<size_t, size_t> heights = count_heights(table);
    for (size_t r = 1; r < table.size(); ++r) {
	size_t current_height = heights.contains(r) ? heights.at(r) : 1;
	std::vector<size_t> newline_pos(table[r].size(), 0);
	for(size_t h = 0; h < current_height; ++h) {
	    out << V_BAR;
	    for (std::size_t c = 0; c < table[r].size(); ++c) {
		auto line_start = newline_pos[c] <= table[r][c].size() ? newline_pos[c] : table[r][c].size();
		newline_pos[c] = table[r][c].find('\n', line_start);
		auto current_substr = table[r][c].substr(line_start, newline_pos[c] - line_start);
		if(newline_pos[c] != StringType::npos) ++newline_pos[c];
		out << SPACE
		    << std::left
		    << std::setw(static_cast<int>(widths[c]))
		    << current_substr
		    << (current_height == 1 || current_substr.empty() ? SPACE : '+')
		    << V_BAR;
	    }
	    out << std::endl;
	}
    }

}

}
