#pragma once
#include "tables_gathered.hpp"
#include "validateable.hpp"
#include "expected_cell_value.hpp"

namespace garlic {

/// Base class for all expressions in queries.
/*! Expression is an entity that can be resolved as number, string or other primitive. */
class Expression : public CanBeValidated<CellType> {
public:
    virtual ~Expression() = default;

    Expression(TypeOrError type_or_err)
    : CanBeValidated{ type_or_err }
    {}
    
    /// Resolves underlying subexpressions or values and forms the @ref CellValue object.
    /*! @throws std::logic_error may throw if did not validate first. */
    virtual ExpectedCellValue resolve(const TablesGathered& gatherers) const = 0;

    using UsedTables = std::unordered_set<TableNameType>;
    /// Inspects AST to get used tables. Used to validate used and FROM-selected tables.
    /*! @throws std::logic_error if did not validate first. */
    virtual UsedTables get_used_tables() const = 0;

protected:
    static UsedTables get_used_tables_from(sptr<Expression> lhs, sptr<Expression> rhs) {
	auto lhs_tables = lhs->get_used_tables();
	auto rhs_tables = rhs->get_used_tables();
	lhs_tables.merge(rhs_tables);
	return lhs_tables;
    }
    static UsedTables get_used_tables_from(sptr<Expression> expr) {
	return expr->get_used_tables();
    }
};

}
