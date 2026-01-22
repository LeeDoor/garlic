#include "table_content.hpp"

namespace garlic {

TableContent::TableContent(size_t row_size_bytes) 
: row_size_bytes_{row_size_bytes}
, content_{}
{}

size_t TableContent::create_empty_row() {
    content_.push_back(ByteArray{row_size_bytes_});
    return content_.size() - 1;
}

void TableContent::set_value(size_t row_id, size_t offset, ByteArray value) {
    if(offset + value.size() > row_size_bytes_)
        throw std::logic_error("offset + value byte array is longer than available in row");
    std::memcpy(content_[row_id].data() + offset, value.data(), value.size());
}

ByteArray TableContent::get_value(size_t row_id, size_t offset, size_t count) {
    if(offset + count > row_size_bytes_)
        throw std::logic_error("requested to read data more than available in row");
    ByteArray result(count);
    std::memcpy(result.data(), content_[row_id].data() + offset, count);
    return result;
}

}
