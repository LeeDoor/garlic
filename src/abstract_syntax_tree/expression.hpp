#pragma once
#include "table_value_gatherer.hpp"
#include "validateable.hpp"
#include "expected_cell_value.hpp"

namespace garlic {

/// Base class for all expressions in queries.
/*! Expression is an entity that can be resolved as number, string or other primitive. */
class Expression : public CanBeValidated {
public:
    virtual ~Expression() = default;

    Expression(TypeRules::TypeOrError type_or_err)
    : CanBeValidated{ type_or_err }
    {}
    
    /// Resolves underlying subexpressions or values and forms the @ref CellValue object.
    /*! @throws std::logic_error may throw if did not validate first. */
    virtual ExpectedCellValue resolve(sptr<TableValueGatherer> gatherer) const = 0;
};

}
