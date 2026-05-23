#include "ready_selector_generator.hpp"

namespace garlic::sql_parser { 

ReadySelectorGenerator::ReadySelectorGenerator(Selector&& selector)
: selector_{ std::move(selector) }
{}

std::expected<std::list<Selector>, StringType> ReadySelectorGenerator::generate(const TablesContainer& ) {
    return std::list<Selector>{ selector_ };
}
Expression::UsedTables ReadySelectorGenerator::get_used_tables() const {
    return selector_.ast->get_used_tables();
}

}
