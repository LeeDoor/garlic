#pragma once
#include "expected_cell_value.hpp"
#include "string_query_result.hpp"

namespace garlic {
class QueryResult;
class TableValueGatherer;

/*! @brief Query defines the root of AST defining any query. 
 *  Query may be resolved, which returns pointer to abstract @ref QueryResult.
 *  Resolving requires @ref TableValueGatherer.
 */
class Query {
public:
    virtual ~Query() = default;

    /// Resolves query and forms the @ref QueryResult on return.
    /*! @param gatherer used to resolve table cells mentioned in query. */
    [[nodiscard]] virtual sptr<QueryResult> resolve(sptr<TableValueGatherer> gatherer) = 0;

protected:
    sptr<QueryResult> execute_error(const UnexpectedCellValue& err) {
	return std::make_unique<StringQueryResult>("[RUNTIME_ERROR] " + err + "\n\n");
    }
};

}
