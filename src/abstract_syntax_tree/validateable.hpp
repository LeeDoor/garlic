#pragma once
#include "type_rules.hpp"

namespace garlic {

using InvalidError = StringType;
using ExpectedValid = std::expected<void, InvalidError>;

class CanBeValidated {
public:
    /// Checks if given node is valid and may be resolved without semantic errors.
    /*! @returns std::optional if expression is valid; StringType with error message overwise. */
    virtual ExpectedValid validate() const = 0;

    /// Returns the type of underlying value or resulting type after some operation
    /// made by this Expression.
    /*! @returns @ref CellType if type is valid. 
     *  @throws std::logic_error if type was not recognized. */
    virtual CellType get_type() const { 
	return type_or_err_.value();
    }

protected:
    CanBeValidated(TypeRules::TypeOrError toe)
    : type_or_err_{ toe }
    {}

    ExpectedValid validate(CellType lhs, std::optional<CellType> rhs = std::nullopt) const {
	if(type_or_err_.has_value())
	    return ExpectedValid{};
	return std::unexpected(TypeRules::write_error(type_or_err_.error(), lhs, rhs));
    }

    TypeRules::TypeOrError type_or_err_;
};

}
