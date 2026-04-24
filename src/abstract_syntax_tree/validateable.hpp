#pragma once
#include "type_rules.hpp"

namespace garlic {

using InvalidError = StringType;
using ExpectedValid = std::expected<void, InvalidError>;

class CanBeValidated {
public:
    CanBeValidated(TypeRules::TypeOrError toe)
    : type_or_err_{ toe }
    {}

    /// Checks if given node is valid and may be resolved without semantic errors.
    /*! @returns nothing if OK or StringType with error message. */
    ExpectedValid validate() const {
	if(type_or_err_.has_value())
	    return ExpectedValid{};
	return std::unexpected(type_or_err_.error());
    }

    /// Returns the type of underlying value or resulting type after some operation
    /// made by this Expression.
    /*! @returns @ref CellType if type is valid. 
     *  @throws std::logic_error if type was not recognized. */
    virtual CellType get_type() const { 
	if(!type_or_err_.has_value()) // Throwing std::logic_error (not std::bad_expected_access)
	    throw std::logic_error("Calling get_type of Validateable object which is invalid");
	return type_or_err_.value();
    }

protected:
    TypeRules::TypeOrError type_or_err_;
};

}
