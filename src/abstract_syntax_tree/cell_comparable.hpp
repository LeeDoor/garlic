#pragma once
#include "cell_value.hpp"

namespace garlic {

/// @ref CellValue subclass defining comparable objects.
class CellComparable : virtual public CellValue {
public:
    /// Equals operator ==
    /*! @param rhs comparement object
     *  @returns true if this's value and other's value are equal.
     *  @throws std::logic_error if trying to compare uncomparable types.
     */
    virtual bool equals(sptr<CellValue> other) const = 0;
    virtual bool ne(sptr<CellValue> other) const { return !equals(other); }
    /// Less or Equal operator <= 
    /*! @param rhs comparement object
     *  @returns true if this's value is less than or equal to other's value.
     *  @throws std::logic_error if trying to compare uncomparable types.
     */
    virtual bool le(sptr<CellValue> other) const = 0;
    /// Less Than operator <
    /*! @param rhs comparement object
     *  @returns true if this's value is less than other's value.
     *  @throws std::logic_error if trying to compare uncomparable types.
     */
    virtual bool lt(sptr<CellValue> other) const = 0;
    /// Greater or Equal operator >=
    /*! @param rhs comparement object
     *  @returns true if this's value is more than or equal to other's value.
     *  @throws std::logic_error if trying to compare uncomparable types.
     */
    virtual bool ge(sptr<CellValue> other) const = 0;
    /// Greater Than operator >
    /*! @param rhs comparement object
     *  @returns true if this's value is more than other's value.
     *  @throws std::logic_error if trying to compare uncomparable types.
     */
    virtual bool gt(sptr<CellValue> other) const = 0;
};

}
