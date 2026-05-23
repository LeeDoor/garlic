#pragma once
#include "selector.hpp"
#include "selector_generator.hpp"

namespace garlic::sql_parser {

class ReadySelectorGenerator : public SelectorGenerator {
public:
    ReadySelectorGenerator(Selector&& selector);

    std::expected<std::list<Selector>, StringType> generate(const TablesContainer& ) override;
    Expression::UsedTables get_used_tables() const override;
    bool requires_from_clause() const override { return false; }
    
private:
    Selector selector_;
};

}
