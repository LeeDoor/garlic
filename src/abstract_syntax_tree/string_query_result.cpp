#include "string_query_result.hpp"

namespace garlic::abstract_syntax_tree {

StringViewType StringQueryResult::format() const { return result_str_; }

} // namespace garlic::abstract_syntax_tree
