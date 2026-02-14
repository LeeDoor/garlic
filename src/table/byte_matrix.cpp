#include "byte_matrix.hpp"

namespace garlic {

ByteMatrix::ByteMatrix(size_t row_size_bytes) 
: row_size_bytes_{row_size_bytes}
, content_{}
{}

size_t ByteMatrix::create_empty_row() {
    content_.push_back(ByteVector(row_size_bytes_));
    return content_.size() - 1;
}

void ByteMatrix::set_value(size_t row_id, size_t offset, ByteVector value) {
    return set_value(row_id, offset, ByteSpan{ value.data(), value.size() });
}

void ByteMatrix::set_value(size_t row_id, size_t offset, ByteSpan value) {
    if(value.empty())
        throw std::logic_error("trying to set an empty value");
    if(offset + value.size() > row_size_bytes_)
        throw std::logic_error("offset + value byte array is longer than available in row");
    if(row_id >= content_.size())
        throw std::logic_error("row_id{" + std::to_string(row_id) + "} is more than rows count{" + std::to_string(content_.size()) + "}");
    std::memcpy(content_[row_id].data() + offset, value.data(), value.size());
}

ByteSpan ByteMatrix::get_value(size_t row_id, size_t offset, size_t count) const {
    if(count == 0)
        throw std::logic_error("trying to read an empty byte array");
    if(offset + count > row_size_bytes_)
        throw std::logic_error("requested to read data more than available in row");
    if(row_id >= content_.size())
        throw std::logic_error("row_id{" + std::to_string(row_id) + "} is more than rows count{" + std::to_string(content_.size()) + "}");
    const ByteSpan result(content_[row_id].begin() + offset, count);
    return result;
}

}
