#pragma once
#include "expression.hpp"
#include "table.hpp"

namespace garlic {

class Selector;

class SelectorGenerator {
public:
    virtual ~SelectorGenerator() = default;

    using Tables = std::list<Table>;

    virtual ExpectedOrStr<std::list<Selector>> generate([[maybe_unused]] const Tables& tables) = 0;
    virtual Expression::UsedTables get_used_tables() const = 0;
    virtual bool requires_from_clause() const = 0;
};

}
