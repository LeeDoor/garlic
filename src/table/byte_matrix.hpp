#pragma once

namespace garlic {

/*!
 * @brief Byte-based table. 
 *
 * All rows have fixed size, amount of rows is unlimited.
 * Used by @ref TypedTable to manage data in the table.
 * This concrete class is not tied with any types. Type
 * matching responsibility lays on @ref TypedTable.
 */
class ByteMatrix {
public:
    /// Defines ByteMatrix with fixed size of each row in bytes.
    /*! @param row_size_bytes amount of bytes in each row */
    ByteMatrix(size_t row_size_bytes);

    /// Creates empty row filled with zeros.
    /*! @returns id of created row */
    size_t create_empty_row();
    /// Changes bytes at rows[row_id] with given offset to bytes
    /// from value array.
    /*! @param row_id id of modifying row (may be returned from create_empty_row).
     *  @param offset byte offset in row OR first byte to start edit from.
     *  @param value byte array with data needed to be written to table.
     *  @throws if row_id is invalid.
     *  @throws if value is empty.
     *  @throws if value doesn't fit in row with given offset.
     */
    void set_value(size_t row_id, size_t offset, ByteSpan value);
    void set_value(size_t row_id, size_t offset, ByteVector value);
    /// Same as set_value, but filling with zeros.
    void clear_value(size_t row_id, size_t offset, size_t count);
    /// Reads N bytes from table at given row with given offset.
    /*!
     * @param row_id id of the row to read from (may be returned from create_empty_row).
     * @param offset byte offset in row OR first byte to start read from.
     * @param count amount of bytes to read.
     * @throws if row_id is invalid.
     * @throws if count and offset are too big to fit in row.
     * @throws if reading zero bytes (count == 0).
     * @returns @ref ByteArray of read bytes. assert(ByteArray.size() == count).
     */
    ByteSpan get_value(size_t row_id, size_t offset, size_t count) const;

private:
    size_t row_size_bytes_;
    std::vector<ByteVector> content_;
};

}
