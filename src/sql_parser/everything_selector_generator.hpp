#pragma once
#include "selector_generator.hpp"
#include "tables_header_gatherer.hpp"

namespace garlic::sql_parser {

class EverythingSelectorGenerator : public garlic::abstract_syntax_tree::SelectorGenerator {
  public:
	EverythingSelectorGenerator(const garlic::abstract_syntax_tree::TablesHeaderGatherer &header_gatherer);

	ExpectedOrStr<std::list<garlic::abstract_syntax_tree::Selector>>
	generate(const garlic::abstract_syntax_tree::SelectorGenerator::Tables &tables) override;
	garlic::abstract_syntax_tree::Expression::UsedTables get_used_tables() const override;
	bool requires_from_clause() const override { return true; }

  private:
	const garlic::abstract_syntax_tree::TablesHeaderGatherer &header_gatherer_;
};

} // namespace garlic::sql_parser
