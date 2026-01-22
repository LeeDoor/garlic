#include "table.hpp"

namespace garlic {

Table::Table(InitList column_headers)
    : row_size_bytes_{calculate_row_size(column_headers)}
, header_{create_column_header(column_headers)}
, content_{row_size_bytes_}
{}

size_t Table::create_empty_row() {
    return content_.create_empty_row();
}

size_t Table::calculate_row_size(const InitList& headers) {
    return std::accumulate(headers.begin(), headers.end(), 0, 
            [](size_t lhs, const PublicColumnInfo& rhs) {
            size_t column_size = 
            rhs.type == String ? 
            rhs.size_characters * sizeof(CharType) : get_type_size(rhs.type);
            return lhs + column_size; 
            });
}

std::vector<ColumnInfo> Table::create_column_header(const InitList& column_headers) {
    std::vector<ColumnInfo> result;
    size_t offset = 0;
    result.reserve(column_headers.size());
    for(auto& column : column_headers) {
        size_t column_size = 
            column.type == String ? 
            column.size_characters * sizeof(CharType) : get_type_size(column.type);
        result.push_back(ColumnInfo { column.type, column.column_name, column_size, offset });
        offset += column_size;
    }
    return result;
}

}
