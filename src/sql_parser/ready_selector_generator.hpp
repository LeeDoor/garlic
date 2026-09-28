#pragma once
#include "selector.hpp"
#include "selector_generator.hpp"

namespace garlic::sql_parser {

class ReadySelectorGenerator : public garlic::abstract_syntax_tree::SelectorGenerator {
  public:
	ReadySelectorGenerator(garlic::abstract_syntax_tree::Selector &&selector);

	ExpectedOrStr<std::list<garlic::abstract_syntax_tree::Selector>>
	generate(const garlic::abstract_syntax_tree::SelectorGenerator::Tables &) override;
	garlic::abstract_syntax_tree::Expression::UsedTables get_used_tables() const override;
	bool requires_from_clause() const override { return false; }

  private:
	garlic::abstract_syntax_tree::Selector selector_;
};

} // namespace garlic::sql_parser
