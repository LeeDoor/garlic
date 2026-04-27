#pragma once
#include "runtime_error.hpp"
#include "table_value_gatherer_factory.hpp"

namespace garlic {
class QueryResult;
class CellValueGatherer;

/*! @brief Query defines the root of AST defining any query. 
 *  Query may be resolved, which returns pointer to abstract @ref QueryResult.
 *  Resolving requires @ref CellValueGatherer.
 */
class Query {
public:
    virtual ~Query() = default;

    using ExpectedQueryResult = std::expected<sptr<QueryResult>, RuntimeError>; 

    /// Resolves query and forms the @ref QueryResult on return.
    /*! @param gatherer used to resolve table cells mentioned in query. */
    [[nodiscard]] virtual ExpectedQueryResult resolve(const TableValueGathererFactory& gatherer_factory) = 0;
};

}
