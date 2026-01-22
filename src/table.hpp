#pragma once
#include "column_info.hpp"
#include "public_column_info.hpp"
#include "table_content.hpp"

namespace garlic {

static std::string ERROR_COLUMN_ID_TOO_BIG = "incoming column id is more than columns amount";
static std::string ERROR_DATA_TYPE_MISMATCH = "trying to read/write wrong type of data";
static std::string ERROR_DATA_SIZE_MISMATCH = "trying to read/write data with wrong size";
class Table {
    using InitList = std::initializer_list<PublicColumnInfo>;
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

    template<typename T> 
    requires std::is_arithmetic_v<T>
    void set_value(size_t row, size_t column, T value) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != get_cell_type<T>())
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);
        if(header_[column].size_bytes != sizeof(value))
            throw std::logic_error(ERROR_DATA_SIZE_MISMATCH);

        size_t row_offset = header_[column].offset;
        ByteArray value_bytes;
        value_bytes.resize(sizeof(T));
        std::memcpy(value_bytes.data(), &value, sizeof(T));
        return content_.set_value(row, row_offset, value_bytes);
    }

    void set_value(size_t row, size_t column, const std::string& value) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != String) 
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);
        if(header_[column].size_bytes < value.size())
            throw std::logic_error(ERROR_DATA_SIZE_MISMATCH);

        size_t row_offset = header_[column].offset;
        ByteArray value_bytes;
        value_bytes.resize(value.size());
        std::memcpy(value_bytes.data(), value.data(), value.size());
        return content_.set_value(row, row_offset, value_bytes);
    }

    template<typename T>
    requires std::is_arithmetic_v<T>
    T get_value(size_t row, size_t column) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != get_cell_type<T>()) 
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);

        T out;
        ByteArray bytes = content_.get_value(row, header_[column].offset, sizeof(T));
        std::memcpy(&out, bytes.data(), sizeof(T));
        return out;
    }
    
    template<typename T>
    requires std::is_same_v<T, std::string>
    std::string get_value(size_t row, size_t column) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != String) 
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);

        ByteArray bytes = content_.get_value(row, header_[column].offset, header_[column].size_bytes);
        std::string out(bytes.size() / sizeof(char), '\0');
        std::memcpy(out.data(), bytes.data(), bytes.size());
        return out;
    }

private:
    template<typename T> requires std::is_same_v<T, int> 
    static CellType get_cell_type() { return CellType::Int; }

    template<typename T> requires std::is_same_v<T, float> 
    static CellType get_cell_type() { return CellType::Float; }

    static size_t calculate_row_size(const InitList& headers) {
        return std::accumulate(headers.begin(), headers.end(), 0, 
            [](size_t lhs, const PublicColumnInfo& rhs) {
                return lhs + rhs.size_bytes;
            });
    }
    static std::vector<ColumnInfo> create_column_header(const InitList& column_headers) {
        std::vector<ColumnInfo> result;
        size_t offset = 0;
        result.reserve(column_headers.size());
        for(auto& column : column_headers) {
            result.push_back(ColumnInfo { column.type, column.column_name, column.size_bytes, offset });
            offset += column.size_bytes;
        }
        return result;
    }


    size_t row_size_bytes_;
    std::vector<ColumnInfo> header_;
    TableContent content_;
};

}
