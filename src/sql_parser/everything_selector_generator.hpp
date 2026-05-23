#pragma once
#include "selector_generator.hpp"
#include "tables_header_gatherer.hpp"

namespace garlic::sql_parser {

class EverythingSelectorGenerator : public SelectorGenerator {
public:
    EverythingSelectorGenerator(const TablesHeaderGatherer& header_gatherer);

    std::expected<std::list<Selector>, StringType> generate(const TablesContainer& tables) override;
    Expression::UsedTables get_used_tables() const override;
    bool requires_from_clause() const override { return true; }

private:
    const TablesHeaderGatherer& header_gatherer_;
};

}
