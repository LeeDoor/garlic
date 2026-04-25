#pragma once
#include "cell_boolean_value.hpp"
#include "expression.hpp"
#include "cell_value_gatherer.hpp"

namespace garlic {

class CellBooleanValue;

/// Base class for all Conditions in query.
/*! Condition is a logical entity that can be resolved as @ref CellBooleanValue. */
class Condition : public Expression {
public:
    virtual ~Condition() = default;

    Condition(TypeRules::TypeOrError toe) : Expression{ toe } {}

    using ExpectedCellBooleanValue = std::expected<sptr<CellBooleanValue>, UnexpectedCellValue>;

    /// Same as @ref Expression::resolve(), but ensures the boolean return type (or error).
    virtual ExpectedCellBooleanValue resolve_bool(sptr<CellValueGatherer> gatherer) const = 0;

private:
    /// Performs logical operations on underlying objects.
    /*! @throws std::logic_error may throw if did not validate first. */
    ExpectedCellValue resolve(sptr<CellValueGatherer> gatherer) const override {
	return resolve_bool(gatherer);
    }
};

}
