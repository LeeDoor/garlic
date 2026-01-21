#pragma once

namespace garlic {

enum CellType {
    String, Int, Float
};

struct IncomingColumnInfo {
    CellType type;
    std::string column_name;
    size_t size_bytes;
};

struct ColumnHeader {
    CellType type;
    std::string column_name;
    size_t size_bytes;
    size_t offset;
};

class TableContent {
    using ByteArray = std::vector<std::byte>;
public:
    TableContent(size_t row_size_bytes) 
    : row_size_bytes_{row_size_bytes}
    , content_{}
    {}

    ssize_t create_empty_row() {
        content_.push_back(ByteArray{row_size_bytes_});
        return content_.size() - 1;
    }

    void set_value(size_t row_id, size_t offset, ByteArray value) {
        if(offset + value.size() > row_size_bytes_)
            throw std::logic_error("offset + value byte array is longer than available in row");
        std::memcpy(content_[row_id].data() + offset, value.data(), value.size());
    }

    ByteArray get_value(size_t row_id, size_t offset, size_t count) {
        if(offset + count > row_size_bytes_)
            throw std::logic_error("requested to read data more than available in row");
        ByteArray result(count);
        std::memcpy(result.data(), content_[row_id].data() + offset, count);
        return result;
    }

private:
    size_t row_size_bytes_;
    std::vector<ByteArray> content_;
};

static std::string ERROR_COLUMN_ID_TOO_BIG = "incoming column id is more than columns amount";
static std::string ERROR_DATA_TYPE_MISMATCH = "trying to read/write wrong type of data";
static std::string ERROR_DATA_SIZE_MISMATCH = "trying to read/write data with wrong size";
class Table {
    using InitList = std::initializer_list<IncomingColumnInfo>;
public:
    Table(InitList column_headers)
    : row_size_bytes_{calculate_row_size(column_headers)}
    , header_{create_column_header(column_headers)}
    , content_{row_size_bytes_}
    {}

    /*! \returns id of created row or -1 if error occured */
    ssize_t create_empty_row() {
        return content_.create_empty_row();
    }

    template<typename T> requires std::is_arithmetic_v<T>
    void set_value(size_t row, size_t column, T value) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != get_cell_type<T>())
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);
        if(header_[column].size_bytes != sizeof(value))
            throw std::logic_error(ERROR_DATA_SIZE_MISMATCH);

        size_t row_offset = header_[column].offset;
        std::vector<std::byte> value_bytes;
        value_bytes.resize(sizeof(T));
        std::memcpy(value_bytes.data(), &value, sizeof(T));
        return content_.set_value(row, row_offset, value_bytes);
    }
    void set_value(size_t row, size_t column, const std::string& value) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != String) 
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);
        if(header_[column].size_bytes != value.size())
            throw std::logic_error(ERROR_DATA_SIZE_MISMATCH);

        size_t row_offset = header_[column].offset;
        std::vector<std::byte> value_bytes;
        value_bytes.resize(value.size());
        std::memcpy(value_bytes.data(), value.data(), value.size());
        return content_.set_value(row, row_offset, value_bytes);
    }

private:
    template<int T> static CellType get_cell_type() { return CellType::Int; }
    template<float T> static CellType get_cell_type() { return CellType::Float; }

    static size_t calculate_row_size(InitList headers) {
        return std::accumulate(headers.begin(), headers.end(), 0, 
            [](size_t lhs, const IncomingColumnInfo& rhs) {
                return lhs + rhs.size_bytes;
            });
    }
    static std::vector<ColumnHeader> create_column_header(InitList column_headers) {
        std::vector<ColumnHeader> result;
        size_t offset = 0;
        result.reserve(column_headers.size());
        for(auto& column : column_headers) {
            result.push_back(ColumnHeader { column.type, column.column_name, column.size_bytes, offset });
            offset += column.size_bytes;
        }
        return result;
    }


    size_t row_size_bytes_;
    std::vector<ColumnHeader> header_;
    TableContent content_;
};

}
