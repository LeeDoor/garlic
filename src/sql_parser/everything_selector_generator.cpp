#include "everything_selector_generator.hpp"
#include "selector.hpp"
#include "table_value_expression.hpp"

namespace garlic::sql_parser { 

EverythingSelectorGenerator::EverythingSelectorGenerator(const TablesHeaderGatherer& header_gatherer)
: header_gatherer_{ header_gatherer }
{}

ExpectedOrStr<std::list<Selector>> EverythingSelectorGenerator::generate(const TablesContainer& tables) {
    std::list<Selector> selectors;
    if(tables.empty())
        return std::unexpected("SELECT * with no tables in FROM clause is an error");
    for(const auto& table : tables) {
        auto header = header_gatherer_.get_tables_header(table.table_name);
        if(!header)
            throw std::logic_error("Generating columns with generators while tables are invalid; check them before generation.");
        for(const auto& column : *header) {
            selectors.push_back(
                Selector{ std::make_shared<TableValueExpression>(header_gatherer_, table.table_name, column.name) }
            );
        }
    }
    return selectors;
}
Expression::UsedTables EverythingSelectorGenerator::get_used_tables() const {
    return {};
}

}
