#pragma once
#include "expression.hpp"
#include "table.hpp"

namespace garlic {

class Selector;

class SelectorGenerator {
public:
    virtual ~SelectorGenerator() = default;

    using TablesContainer = std::list<Table>;

    virtual std::expected<std::list<Selector>, StringType> generate([[maybe_unused]] const TablesContainer& tables) = 0;
    virtual Expression::UsedTables get_used_tables() const = 0;
    virtual bool requires_from_clause() const = 0;
};

}
