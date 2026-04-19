#pragma once
#include "cell_boolean_value.hpp"
#include "expression.hpp"
#include "table_value_gatherer.hpp"

namespace garlic {

class CellBooleanValue;

/// Base class for all Conditions in query.
/*! Condition is a logical entity that can be resolved as True or False. */
class Condition : public Expression {
public:
    virtual ~Condition() = default;

    Condition(TypeRules::TypeOrError toe) : Expression{ toe } {}

    using ExpectedCellBooleanValue = std::expected<sptr<CellBooleanValue>, UnexpectedCellValue>;

    virtual ExpectedCellBooleanValue resolve_bool(sptr<TableValueGatherer> gatherer) const = 0;

private:
    /// Performs logical operations on underlying objects.
    /*! @throws std::logic_error may throw if did not validate first. */
    ExpectedCellValue resolve(sptr<TableValueGatherer> gatherer) const override {
	return resolve_bool(gatherer);
    }
};

}
