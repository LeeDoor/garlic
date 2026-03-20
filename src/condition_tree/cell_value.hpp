#pragma once
#include "cell_type.hpp"
#include "get_cell_primitive.hpp"
#include "get_cell_type.hpp"

namespace garlic {

const std::string TYPE_MISMATCH = "calling get_<INT|FLOAT|STRING> to a function which does not match this data type.";

/// Multitype value wrapper. Populated by @ref Expression .
/*! Base class for values of each type. One child - one type. 
 *  Child should implement comparing operations with other object.
 */
class CellValue {
public:
    using CellValuePtr = std::shared_ptr<CellValue>;

    virtual ~CellValue() = default;

    /// Equals operator ==
    /*! @param rhs comparement object
     *  @returns true if this's value and other's value are equal.
     *  @throws std::logic_error if trying to compare uncomparable types.
     */
    virtual bool equals(CellValuePtr other) const = 0;
    /// Less or Equal operator <= 
    /*! @param rhs comparement object
     *  @returns true if this's value is less than or equal to other's value.
     *  @throws std::logic_error if trying to compare uncomparable types.
     */
    virtual bool le(CellValuePtr other) const = 0;
    /// Less Than operator <
    /*! @param rhs comparement object
     *  @returns true if this's value is less than other's value.
     *  @throws std::logic_error if trying to compare uncomparable types.
     */
    virtual bool lt(CellValuePtr other) const = 0;
    /// Greater or Equal operator >=
    /*! @param rhs comparement object
     *  @returns true if this's value is more than or equal to other's value.
     *  @throws std::logic_error if trying to compare uncomparable types.
     */
    virtual bool ge(CellValuePtr other) const = 0;
    /// Greater Than operator >
    /*! @param rhs comparement object
     *  @returns true if this's value is more than other's value.
     *  @throws std::logic_error if trying to compare uncomparable types.
     */
    virtual bool gt(CellValuePtr other) const = 0;

    /// Get the enum type of underlying object.
    CellType get_type() const { return cell_type_; }

    /// Print value to std::ostream.
    virtual void format(std::ostream& os) const = 0;
protected:
    CellValue(CellType cell_type) : cell_type_{ cell_type } {}
    
    template<typename CellValueType> 
    requires std::is_base_of_v<CellValue, CellValueType>
    static bool is_type(CellValuePtr other) {
        return as_casted_ptr<CellValueType>(other) != nullptr;
    }

    template<IsAnyColumnType T> 
    static T to_type(CellValuePtr other) {
        auto ptr = as_casted_ptr<typename get_cell_type<T>::Type>(other);
	if(ptr == nullptr) 
	    throw std::logic_error("Trying to cast uncastable type");
	return static_cast<T>(*ptr);
    }

    template<typename CellValueType> 
    requires std::is_base_of_v<CellValue, CellValueType>
    static std::shared_ptr<CellValueType> as_casted_ptr(CellValuePtr other) {
        return std::dynamic_pointer_cast<CellValueType>(other);
    }
    
    template<typename CellValueType> 
    requires std::is_base_of_v<CellValue, CellValueType>
    static std::shared_ptr<CellValueType> make_copy(typename get_cell_primitive<CellValueType>::Type value) {
	return std::make_shared<CellValueType>(value);
    }

    CellType cell_type_;
};

}
