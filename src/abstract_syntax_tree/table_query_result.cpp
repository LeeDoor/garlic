#include "table_query_result.hpp"

namespace garlic {

TableQueryResult::TableQueryResult(ResultTable&& table) 
: table_result_{ TableQueryResultGenerator::form_table_result(table) }
{}

StringViewType TableQueryResult::format() const {
    return table_result_;
}

TableQueryResultGenerator::TableQueryResultGenerator(const ResultTable& table) 
: widths_{ count_widths(table) }
, heights_{ count_heights(table) }
, table_{ table }
, out_{ }
{}
StringType TableQueryResultGenerator::form_table_result() {
    print_table();
    return out_.str();
}

void TableQueryResultGenerator::print_table() {
    if (table_.empty()) { return; }
    print_horizontal_delimeter();
    print_table_header();
    print_horizontal_delimeter();
    print_table_body();
    if(table_.size() > 1)
	print_horizontal_delimeter();
}

std::vector<size_t> TableQueryResultGenerator::count_widths(const ResultTable& table) {
    const std::size_t cols = table[0].size();
    std::vector<std::size_t> widths(cols, 0);
    for (const auto& row : table) {
        for (std::size_t i = 0; i < cols; ++i) {
	    auto& cell = row[i];
	    size_t l = 0, r = 0;
	    do {
		auto next_eol = cell.find('\n', r);
		if(next_eol == StringType::npos)
		    next_eol = cell.size();
		l = r;
		r = next_eol;
		widths[i] = std::max(widths[i], r - l);
		if(r != cell.size())
		    ++r;
	    } while(r != cell.size());
        }
    }
    return widths;
}
std::unordered_map<size_t, size_t> TableQueryResultGenerator::count_heights(const ResultTable& table) {
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

void TableQueryResultGenerator::print_horizontal_delimeter() {
    out_ << CROSS;
    for (std::size_t i = 0; i < widths_.size(); ++i) {
        out_ << std::string(widths_[i] + 2, H_BAR)
	     << CROSS;
    }
    out_ << std::endl;
}

void TableQueryResultGenerator::print_table_header() {
    print_row(0);
}

void TableQueryResultGenerator::print_table_body() {
    for (size_t row_id = 1; row_id < table_.size(); ++row_id) {
	print_row(row_id);
    }
}

void TableQueryResultGenerator::print_row(size_t row_id) {
    auto& row = table_[row_id];
    auto height = get_height(row_id);
    std::vector<size_t> newline_pos(row.size(), 0);
    for(size_t h = 0; h < height; ++h) {
	out_ << V_BAR;
	for (std::size_t cell_id = 0; cell_id < row.size(); ++cell_id) {
	    auto [current_cell_subline, is_last_subline] = get_cell_subline(newline_pos, row_id, cell_id);
	    print_cell_subline(current_cell_subline, cell_id, is_last_subline);
	    out_ << V_BAR;
	}
	out_ << std::endl;
    }
}

std::tuple<StringViewType, bool> TableQueryResultGenerator::get_cell_subline(std::vector<size_t>& newline_pos, size_t row_id, size_t cell_id) {
    auto& cell = table_[row_id][cell_id];
    auto line_start = newline_pos[cell_id] <= cell.size() ? newline_pos[cell_id] : cell.size();
    newline_pos[cell_id] = cell.find('\n', line_start);
    StringViewType subline = StringViewType{cell}.substr(line_start, newline_pos[cell_id] - line_start);
    bool is_last_subline = newline_pos[cell_id] == StringType::npos;
    if(!is_last_subline)
	++newline_pos[cell_id];
    return { subline, is_last_subline };
}

void TableQueryResultGenerator::print_cell_subline(const StringViewType& cell_str, size_t column_id, bool is_last_subline) {
    out_ << SPACE
	<< std::left
	<< std::setw(static_cast<int>(widths_[column_id]))
	<< cell_str
	<< (is_last_subline? SPACE : ETC);
}

size_t TableQueryResultGenerator::get_height(size_t row) {
    return heights_.contains(row) ? heights_.at(row) : 1;
}


}
