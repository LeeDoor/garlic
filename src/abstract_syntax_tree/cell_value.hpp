#pragma once
#include "cell_type.hpp"
#include "get_cell_primitive.hpp"
#include "get_cell_type.hpp"

namespace garlic {

const std::string TYPE_MISMATCH = "calling get_<INT|FLOAT|STRING> to a function which does not match this data type.";

/// Multitype value wrapper. Populated by @ref Expression .
/*! Base class for values of each type. One child - one type. 
 *  Child should implement comparing operations with other object.
 *  To get the underlying type name, use:
 *  ```
 *  typename get_cell_type<IntType>::Type // AKA @ref CellIntValue
 *  ```
 *  To get the typename of CellValue child storing some primitive, use:
 *  ```
 *  typename get_cell_primitive<CellIntValue>::Type // AKA IntType
 *  typename get_cell_primitive<CellStringViewValue>::Type // AKA StringViewType
 *  ```
 */
class CellValue {
public:
    virtual ~CellValue() = default;

    /// Get the enum type of underlying object.
    CellType get_type() const { return cell_type_; }

    /// Print value to std::ostream.
    virtual void format(std::ostream& os) const = 0;
protected:
    CellValue(CellType cell_type) : cell_type_{ cell_type } {}
    
    /// Checks if given CellValue is an instance of some child type.
    template<typename CellValueType> 
    requires std::is_base_of_v<CellValue, CellValueType>
    static bool is_type(sptr<CellValue> other) {
        return as_casted_ptr<CellValueType>(other) != nullptr;
    }

    /// Gathers underling value from Cellvalue object.
    /*! @returns underlying object.
     *  @throws std::logic_error if wrong type gathered.
     */
    template<IsAnyColumnType T> 
    static T to_type(sptr<CellValue> other) {
        auto ptr = as_casted_ptr<typename get_cell_type<T>::Type>(other);
	if(ptr == nullptr) 
	    throw std::logic_error("Trying to cast uncastable type");
	return static_cast<T>(*ptr);
    }

    /// Converts given pointer to child CellValue pointer.
    /*! @throws nothing.
     *  @returns shared pointer to converted CellValue <b>or nullptr if failed</b>.
     */
    template<typename CellValueType> 
    requires std::is_base_of_v<CellValue, CellValueType>
    static sptr<CellValueType> as_casted_ptr(sptr<CellValue> other) {
        return std::dynamic_pointer_cast<CellValueType>(other);
    }

    CellType cell_type_;
};

}
