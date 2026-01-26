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
    requires std::is_arithmetic_v<T> && IsColumnType<T>
    void set_value(size_t row, size_t column, T value) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != get_cell_type<T>())
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);
        if(header_[column].size_bytes != sizeof(value))
            throw std::logic_error(ERROR_DATA_SIZE_MISMATCH);

        size_t row_offset = header_[column].offset;
        auto byte_represent = ByteSpan{ reinterpret_cast<const Byte*>(&value), sizeof(T) };
        return content_.set_value(row, row_offset, byte_represent);
    }

    void set_value(size_t row, size_t column, const char value[]) {
        return set_value(row, column, std::string(value));
    }
    
    void set_value(size_t row, size_t column, const StringType& value) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != String) 
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);
        if(header_[column].size_bytes < value.size())
            throw std::logic_error(ERROR_DATA_SIZE_MISMATCH);

        size_t row_offset = header_[column].offset;
        auto string_byte_represent = reinterpret_cast<const Byte*>(value.data());
        return content_.set_value(row, row_offset, ByteSpan{ string_byte_represent, value.size() });
    }

    template<typename T>
    requires std::is_arithmetic_v<T> && IsColumnType<T>
    T get_value(size_t row, size_t column) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != get_cell_type<T>()) 
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);

        T out;
        ByteSpan bytes = content_.get_value(row, header_[column].offset, sizeof(T));
        std::memcpy(&out, bytes.data(), sizeof(T));
        return out;
    }

    template<typename T>
    requires std::is_same_v<T, StringType>
    StringViewType get_value(size_t row, size_t column) {
        return get_value<StringViewType>(row, column);
    }
    template<typename T>
    requires std::is_same_v<T, StringViewType>
    T get_value(size_t row, size_t column) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != String) 
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);

        ByteSpan bytes = content_.get_value(row, header_[column].offset, header_[column].size_bytes);
        StringViewType str = reinterpret_cast<const char*>(bytes.data());
        return str;
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
