#pragma once
#include "column_info.hpp"
#include "table_column_type_gatherer.hpp"
#include "type_rules.hpp"
#include "public_column_info.hpp"
#include "byte_matrix.hpp"

namespace garlic {

static std::string ERROR_COLUMN_ID_TOO_BIG = "incoming column id is more than columns amount";
static std::string ERROR_DATA_TYPE_MISMATCH = "trying to read/write wrong type of data";
static std::string ERROR_DATA_SIZE_MISMATCH = "trying to read/write data with wrong size";

/// Check if Iter is an iterator with type T inside.
template<typename Iter, typename T>
concept IteratorOf = requires(Iter iter) {
    requires std::input_iterator<Iter>;
    requires std::is_same_v<std::iter_value_t<Iter>, T>;
};

/// Check if Container gives access to begin() and end() iterator objects,
/// which are @ref IteratorOf objects.
template<typename Container, typename T>
concept IterableContainer = requires(Container c) {
    { c.begin() } -> IteratorOf<T>;
    { c.end() } -> IteratorOf<T>;
};

/// Type-based table.
/*!
 * Uses @ref ByteMatrix as a container to store data in row manner.
 * Limits user to manage cells verifying data types and byte sizes.
 */
class TypedTable {
public:
    /// Testing override for initializer list. Unpreferrable in project.
    TypedTable(std::initializer_list<PublicColumnInfo> container);

    /// Override for iterable containers.
    template<IterableContainer<PublicColumnInfo> Container>
    TypedTable(const Container& container) 
    : TypedTable(container.begin(), container.end()) {}

    /// Requires any iterable container iterators with info about columns.
    template<IteratorOf<PublicColumnInfo> Iter>
    TypedTable(Iter begin, Iter end)
    : row_size_bytes_{ calculate_row_bytes(begin, end) }
    , header_{ create_column_header(begin, end) }
    , column_name_to_idx_{ hash_column_names(begin, end) }
    , content_{ row_size_bytes_ }
    {}

    /// Returns index of the column with given name.
    /*! @throws std::logic_error if no such column in table. */
    size_t get_column_number_by_name(const std::string& column_name) const;

    /// Returns @ref CellType type of column with given id.
    /*! @throws std::logic_error if column parameter is invalid. */
    CellType get_column_type(size_t column) const;
    CellType get_column_type(const ColumnNameType& column) const { 
	return get_column_type(get_column_number_by_name(column));
    }

    /// Creates empty row. You can't access a row without creating it.
    size_t create_empty_row();

    /// Sets some value T to given row at given column. Overload for numbers.
    /*! @param row index of row that needs a change.
     *  @param column index of cell in row (NOT byte position).
     *  @param value given value with arithmetic type, which should match selected column type.
     *  @throws std::logic_error if column is too big; if type mismatches; if row is too big.
     */
    template<IsStoringColumnType T> 
    requires std::is_arithmetic_v<T>
    void set_value(size_t row, size_t column, T value) {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != get_cell_type<T>())
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);

        size_t row_offset = header_[column].offset;
        auto byte_represent = ByteSpan{ reinterpret_cast<const Byte*>(&value), sizeof(T) };
        return content_.set_value(row, row_offset, byte_represent);
    }

    /// Overload for const char[]. Used for tests.
    void set_value(size_t row, size_t column, const char value[]);
    
    /// Overload for the String type. 
    /*! @param row index of row that needs a change.
     *  @param column index of cell in row (NOT byte position).
     *  @param value string value to set. Should have same string size, as required, or less.
     *  @throws std::logic_error if column is too big; if column type is not string; if string
     *  size mismatches; if row is too big.
     */
    void set_value(size_t row, size_t column, const StringType& value);

    /// Overload for numbers. Returns value at given position.
    /*! @param row index of row to change.
     *  @param cell index in row to change.
     *  @throws std::logic_error if column is too big; if row is too big;
     *  if type T mismatches with actual type.
     */
    template<IsStoringColumnType T>
    requires std::is_arithmetic_v<T>
    T get_value(size_t row, size_t column) const {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != get_cell_type<T>()) 
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);

        ByteSpan bytes = content_.get_value(row, header_[column].offset, sizeof(T));
        T out;
        std::memcpy(&out, bytes.data(), sizeof(T));
        return out;
    }

    /// Overload for strings.
    /*! If @ref StringType is passed as a parameter, function still returns @ref StringViewType. */
    template<typename T>
    requires std::is_same_v<T, StringType>
    StringViewType get_value(size_t row, size_t column) const {
        return get_value<StringViewType>(row, column);
    }
    /// Overload for strings.
    /*! To avoid copying, getting string proceeds with std::string_view. If used String as T instead,
     * other function overload is called and StringView still used.
     * @param row index of row to change.
     * @param cell index in row to change.
     * @throws std::logic_error if row is too big; if column is too big.
     */
    template<typename T>
    requires std::is_same_v<T, StringViewType>
    T get_value(size_t row, size_t column) const {
        if(column >= header_.size()) 
            throw std::logic_error(ERROR_COLUMN_ID_TOO_BIG);
        if(header_[column].type != String) 
            throw std::logic_error(ERROR_DATA_TYPE_MISMATCH);

        ByteSpan bytes = content_.get_value(row, header_[column].offset, header_[column].size_bytes);
        // Finding string size using O(n) traversal; #TODO Store string size in special cell?
        size_t string_size = std::find(bytes.data(), bytes.data() + header_[column].size_bytes, '\0') - bytes.data();
        StringViewType str { reinterpret_cast<const char*>(bytes.data()), string_size };
        return str;
    }

private:
    /// Static function to get @ref CellType enum value from integer type.
    template<typename T> requires std::is_same_v<T, IntType> 
    static CellType get_cell_type() { return CellType::Int; }

    /// Static function to get @ref CellType enum value from float type.
    template<typename T> requires std::is_same_v<T, FloatType> 
    static CellType get_cell_type() { return CellType::Float; }

    /// Calculates the amount of bytes in a row.
    template<IteratorOf<PublicColumnInfo> Iter>
    static size_t calculate_row_bytes(Iter begin, Iter end) {
        auto row_size = std::accumulate(begin, end, 0, 
            [](size_t lhs, const PublicColumnInfo& rhs) {
                size_t column_size = 
                rhs.type == String ? 
                rhs.size_characters * sizeof(CharType) : TypeRules::get_type_size(rhs.type);
                return lhs + column_size; 
            });
        if(row_size == 0)
            throw std::logic_error("Cannot create a table with 0 row size");
        return row_size;
    }

    /// Initializes the vector of ColumnInfo based on PublicColumnInfo container.
    template<IteratorOf<PublicColumnInfo> Iter>
    static std::vector<ColumnInfo> create_column_header(Iter begin, Iter end) {
        std::vector<ColumnInfo> result;
        size_t offset = 0;
        result.reserve(std::distance(begin, end));
        std::unordered_set<std::string> taken_col_names;
        std::for_each(begin, end, [&] (auto& column) {
            size_t column_size = 
                column.type == String ? 
                column.size_characters * sizeof(CharType) : TypeRules::get_type_size(column.type);
            if(taken_col_names.contains(column.name))
                throw std::logic_error("Cannot have two columns with same names");
            taken_col_names.insert(column.name);
            result.push_back(ColumnInfo{ column.type, column.name, column_size, offset });
            offset += column_size;
        });
        return result;
    }

    /// Ties column name hash with it's index in the @ref header_.
    template<IteratorOf<PublicColumnInfo> Iter>
    static std::unordered_map<ColumnNameType, size_t> hash_column_names(Iter begin, Iter end) {
        std::unordered_map<ColumnNameType, size_t> result;
        size_t idx = 0;
        std::for_each(begin, end, [&](auto& column) {
            result[column.name] = idx++;
        });
        return result;
    }

    size_t row_size_bytes_;
    std::vector<ColumnInfo> header_;
    std::unordered_map<ColumnNameType, size_t> column_name_to_idx_;
    ByteMatrix content_;
};

static_assert(TableColumnTypeGatherer<TypedTable>, "TypedTable doesn't match the TableHeaderGatherer concept");

}
