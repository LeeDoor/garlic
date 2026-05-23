#pragma once
#include "runtime_error.hpp"
#include "table_value_gatherer_factory.hpp"
#include "validateable.hpp"

namespace garlic {
class QueryResult;
class CellValueGatherer;

/*! @brief Query defines the root of AST defining any query. 
 *  Query may be resolved, which returns pointer to abstract @ref QueryResult.
 *  Resolving requires @ref CellValueGatherer.
 */
class Query : public CanBeValidated<void> {
public:
    virtual ~Query() = default;

    using ExpectedQueryResult = ExpectedOrStr<sptr<QueryResult>>; 

    Query(CanBeValidated<void>::TypeOrError&& error) 
    : CanBeValidated<void>{ std::move(error) } {}

    /// Resolves query and forms the @ref QueryResult on return.
    /*! @param gatherer used to resolve table cells mentioned in query. */
    [[nodiscard]] virtual ExpectedQueryResult resolve(const TableValueGathererFactory& gatherer_factory) = 0;
};

}
