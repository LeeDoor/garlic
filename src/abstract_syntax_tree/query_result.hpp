#pragma once
#include "cell_type.hpp"

namespace garlic::abstract_syntax_tree {

using namespace garlic::table;

/// Abstract class used to specify output from any resolved query.
/// It may be a single string @ref StringQueryResult, @ref TableQueryResult, etc.
class QueryResult {
  public:
	virtual ~QueryResult() = default;

	/// Virtual function to format underlying
	/// result to StringView object to show in CLI.
	virtual StringViewType format() const = 0;
};

} // namespace garlic::abstract_syntax_tree
