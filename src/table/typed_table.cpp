#include "typed_table.hpp"

namespace garlic {

TypedTable::TypedTable(std::initializer_list<PublicColumnInfo> container) 
: TypedTable(container.begin(), container.end()) {}

size_t TypedTable::get_column_number_by_name(const std::string& column_name) const {
    auto find_result = 
	std::find_if(header_.begin(), header_.end(), [&](ColumnInfo ci) {
	    return ci.name == column_name;
	});
    if(find_result == header_.end()) 
	throw std::logic_error("trying to get id of incorrect column name");
    return std::distance(header_.begin(), find_result);
}

CellType TypedTable::get_column_type(size_t column) const {
    if(column >= header_.size())
	throw std::logic_error("trying to get column type with invalid column");
    return header_[column].type;
}

size_t TypedTable::create_empty_row() {
    return content_.create_empty_row();
}
void TypedTable::set_value(size_t row, size_t column, const char value[]) {
    return set_value(row, column, std::string(value));
}

void TypedTable::set_value(size_t row, size_t column, const StringType& value) {
    if(column >= header_.size()) 
	throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
    if(header_[column].type != String) 
	throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);
    if(header_[column].size_bytes < value.size())
	throw std::logic_error(ERROR_DATA_SIZE_MISMATCH);

    size_t row_offset = header_[column].offset;
    auto string_byte_represent = reinterpret_cast<const Byte*>(value.data());
    content_.set_value(row, row_offset, ByteSpan{ string_byte_represent, value.size() });
    row_offset += value.size();
    content_.clear_value(row, row_offset, header_[column].size_bytes - row_offset);
}

}
