#pragma once
#include "cell_type.hpp"
#include "condition_resolver.hpp"

namespace garlic {

const std::string TYPE_MISMATCH = "calling get_<INT|FLOAT|STRING> to a function which does not match this data type.";

/// Multitype value wrapper. Populated by @ref Expression .
/*! Base class for values of each type. One child - one type. 
 *  Child should implement comparing operations with other object.
 */
class CellValue {
protected:
    using CellValuePtr = std::shared_ptr<CellValue>;
    using ConditionResolverPtr = std::shared_ptr<ConditionResolver>;
public:
    virtual ~CellValue() = default;

    /// Equals operator ==
    /*! @param rhs comparement object
     *  @returns true if this's value and other's value are equal.
     *  @throws std::logic_error if trying to compare different types.
     */
    virtual bool equals(CellValuePtr other) = 0;
    /// Less or Equal operator <= 
    /*! @param rhs comparement object
     *  @returns true if this's value is less than or equal to other's value.
     *  @throws std::logic_error if trying to compare different types.
     */
    virtual bool le(CellValuePtr other) = 0;
    /// Less Than operator <
    /*! @param rhs comparement object
     *  @returns true if this's value is less than other's value.
     *  @throws std::logic_error if trying to compare different types.
     */
    virtual bool lt(CellValuePtr other) = 0;
    /// Greater or Equal operator >=
    /*! @param rhs comparement object
     *  @returns true if this's value is more than or equal to other's value.
     *  @throws std::logic_error if trying to compare different types.
     */
    virtual bool ge(CellValuePtr other) = 0;
    /// Greater Than operator >
    /*! @param rhs comparement object
     *  @returns true if this's value is more than other's value.
     *  @throws std::logic_error if trying to compare different types.
     */
    virtual bool gt(CellValuePtr other) = 0;

    /// Get the enum type of underlying object.
    CellType get_type() { return cell_type_; }

protected:
    CellValue(CellType cell_type) : cell_type_{ cell_type } {}
    
    CellType cell_type_;
};

}
