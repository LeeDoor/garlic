#include "ready_selector_generator.hpp"

namespace garlic::sql_parser {

ReadySelectorGenerator::ReadySelectorGenerator(garlic::abstract_syntax_tree::Selector &&selector)
    : selector_{std::move(selector)} {}

ExpectedOrStr<std::list<garlic::abstract_syntax_tree::Selector>>
ReadySelectorGenerator::generate(const garlic::abstract_syntax_tree::SelectorGenerator::Tables &) {
	return std::list<garlic::abstract_syntax_tree::Selector>{selector_};
}
garlic::abstract_syntax_tree::Expression::UsedTables ReadySelectorGenerator::get_used_tables() const {
	return selector_.ast->get_used_tables();
}

} // namespace garlic::sql_parser
