#include "everything_selector_generator.hpp"
#include "selector.hpp"
#include "table_value_expression.hpp"

namespace garlic::sql_parser {

EverythingSelectorGenerator::EverythingSelectorGenerator(
    const garlic::abstract_syntax_tree::TablesHeaderGatherer &header_gatherer)
    : header_gatherer_{header_gatherer} {}

ExpectedOrStr<std::list<garlic::abstract_syntax_tree::Selector>>
EverythingSelectorGenerator::generate(const garlic::abstract_syntax_tree::SelectorGenerator::Tables &tables) {
	std::list<garlic::abstract_syntax_tree::Selector> selectors;
	if (tables.empty())
		return std::unexpected("SELECT * with no tables in FROM clause is an error");
	for (const auto &table : tables) {
		auto header = header_gatherer_.get_tables_header(table.table_name);
		if (!header)
			throw std::logic_error(
			    "Generating columns with generators while tables are invalid; check them before generation.");
		for (const auto &column : *header) {
			selectors.push_back(garlic::abstract_syntax_tree::Selector{
			    std::make_shared<garlic::abstract_syntax_tree::TableValueExpression>(header_gatherer_, table.table_name,
			                                                                         column.name)});
		}
	}
	return selectors;
}
garlic::abstract_syntax_tree::Expression::UsedTables EverythingSelectorGenerator::get_used_tables() const { return {}; }

} // namespace garlic::sql_parser
