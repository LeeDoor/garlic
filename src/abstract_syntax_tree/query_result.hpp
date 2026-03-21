#pragma once
#include "cell_type.hpp"

namespace garlic {

/// Abstract class used to specify output from any resolved query.
/// It may be a single string @ref StringQueryResult, a table, etc.
class QueryResult {
public:
    virtual ~QueryResult() = default;

    /// Virtual function to format underlying 
    /// result to StringView object to show in CLI.
    virtual StringViewType format() const = 0;
};

}
