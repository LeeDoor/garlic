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
    virtual ~CellValue() = default;

    /// Equals operator ==
    /*! @param rhs comparement object
     *  @returns true if this's value and other's value are equal.
     *  @throws std::logic_error if trying to compare uncomparable types.
     */
    virtual bool equals(sptr<CellValue> other) const = 0;
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

    /// Get the enum type of underlying object.
    CellType get_type() const { return cell_type_; }

    /// Print value to std::ostream.
    virtual void format(std::ostream& os) const = 0;
protected:
    CellValue(CellType cell_type) : cell_type_{ cell_type } {}
    
    template<typename CellValueType> 
    requires std::is_base_of_v<CellValue, CellValueType>
    static bool is_type(sptr<CellValue> other) {
        return as_casted_ptr<CellValueType>(other) != nullptr;
    }

    template<IsAnyColumnType T> 
    static T to_type(sptr<CellValue> other) {
        auto ptr = as_casted_ptr<typename get_cell_type<T>::Type>(other);
	if(ptr == nullptr) 
	    throw std::logic_error("Trying to cast uncastable type");
	return static_cast<T>(*ptr);
    }

    template<typename CellValueType> 
    requires std::is_base_of_v<CellValue, CellValueType>
    static sptr<CellValueType> as_casted_ptr(sptr<CellValue> other) {
        return std::dynamic_pointer_cast<CellValueType>(other);
    }
    
    template<typename CellValueType> 
    requires std::is_base_of_v<CellValue, CellValueType>
    static sptr<CellValueType> make_copy(typename get_cell_primitive<CellValueType>::Type value) {
	return std::make_shared<CellValueType>(value);
    }

    CellType cell_type_;
};

}
