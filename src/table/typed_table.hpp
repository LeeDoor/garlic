#pragma once
#include "column_info.hpp"
#include "public_column_info.hpp"
#include "byte_matrix.hpp"

namespace garlic {

static std::string ERROR_COLUMN_ID_TOO_BIG = "incoming column id is more than columns amount";
static std::string ERROR_DATA_TYPE_MISMATCH = "trying to read/write wrong type of data";
static std::string ERROR_DATA_SIZE_MISMATCH = "trying to read/write data with wrong size";

template<typename Iter, typename T>
concept IteratorOf = requires(Iter iter) {
    requires std::input_iterator<Iter>;
    requires std::is_same_v<std::iter_value_t<Iter>, T>;
};

template<typename Container, typename T>
concept IterableContainer = requires(Container c) {
    { c.begin() } -> IteratorOf<T>;
    { c.end() } -> IteratorOf<T>;
};

class TypedTable {
public:
    TypedTable(std::initializer_list<PublicColumnInfo> container) 
    : TypedTable(container.begin(), container.end()) {}

    template<IterableContainer<PublicColumnInfo> Container>
    TypedTable(const Container& container) 
    : TypedTable(container.begin(), container.end()) {}

    template<IteratorOf<PublicColumnInfo> Iter>
    TypedTable(Iter begin, Iter end)
    : row_size_bytes_{calculate_row_size(begin, end)}
    , header_{create_column_header(begin, end)}
    , content_{row_size_bytes_}
    {}

    size_t create_empty_row() {
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

    template<typename T>
    requires std::is_same_v<T, StringType>
    void set_value(size_t row, size_t column, const T& value) {
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
    requires std::is_same_v<T, StringType>
    StringType get_value(size_t row, size_t column) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != String) 
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);

        ByteArray bytes = content_.get_value(row, header_[column].offset, header_[column].size_bytes);
        StringType out(bytes.size() / sizeof(CharType), '\0');
        std::memcpy(out.data(), bytes.data(), bytes.size());
        return out;
    }

private:
    template<typename T> requires std::is_same_v<T, IntType> 
    static CellType get_cell_type() { return CellType::Int; }

    template<typename T> requires std::is_same_v<T, FloatType> 
    static CellType get_cell_type() { return CellType::Float; }

    template<IteratorOf<PublicColumnInfo> Iter>
    static size_t calculate_row_size(Iter begin, Iter end) {
        auto row_size = std::accumulate(begin, end, 0, 
            [](size_t lhs, const PublicColumnInfo& rhs) {
                size_t column_size = 
                rhs.type == String ? 
                rhs.size_characters * sizeof(CharType) : get_type_size(rhs.type);
                return lhs + column_size; 
            });
        if(row_size == 0)
            throw std::logic_error("Cannot create a table with 0 row size");
        return row_size;
    }

    template<IteratorOf<PublicColumnInfo> Iter>
    static std::vector<ColumnInfo> create_column_header(Iter begin, Iter end) {
        std::vector<ColumnInfo> result;
        size_t offset = 0;
        result.reserve(std::distance(begin, end));
        std::unordered_set<std::string> taken_col_names;
        for(; begin != end; ++begin) {
            auto& column = *begin;
            size_t column_size = 
                column.type == String ? 
                column.size_characters * sizeof(CharType) : get_type_size(column.type);
            if(taken_col_names.contains(column.name))
                throw std::logic_error("Cannot have two columns with same names");
            taken_col_names.insert(column.name);
            result.push_back(ColumnInfo { column.type, column.name, column_size, offset });
            offset += column_size;
        }
        return result;
    }

    size_t row_size_bytes_;
    std::vector<ColumnInfo> header_;
    ByteMatrix content_;
};

}
