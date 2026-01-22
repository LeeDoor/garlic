#pragma once

namespace garlic {

class TableContent {
public:
    TableContent(size_t row_size_bytes) ;

    size_t create_empty_row();
    void set_value(size_t row_id, size_t offset, ByteArray value);
    ByteArray get_value(size_t row_id, size_t offset, size_t count);

private:
    size_t row_size_bytes_;
    std::vector<ByteArray> content_;
};

}
