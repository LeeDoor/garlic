#include "select_query.hpp"
#include "table_query_result.hpp"
#include "table_value_gatherer_factory.hpp"

namespace garlic {

SelectQuery::SelectQuery()
: SelectQuery{ {}, {} }
{}
SelectQuery::SelectQuery(SelectorGenerators selector_gens)
: SelectQuery{ std::move(selector_gens), {} }
{ }
SelectQuery::SelectQuery(SelectorGenerators selector_gens, TablesContainer tables)
: Query{ is_valid(selector_gens, tables) }
, tables_{ std::move(tables) }
, selector_generators_{ std::move(selector_gens) }
{}

CanBeValidated<void>::TypeOrError SelectQuery::is_valid(const SelectorGenerators& generators, const TablesContainer& tables) {
    if(auto err = check_no_same_tables(tables); !err)
        return err;
    if(auto err = check_all_tables_specified(generators, tables); !err)
        return err;
    if(auto err = check_if_from_exists_when_required(generators, tables); !err)
        return err;
    return {};
}
CanBeValidated<void>::TypeOrError SelectQuery::check_no_same_tables(const TablesContainer& tables) {
    std::unordered_set<TableNameType> met_names;
    for(const auto& table : tables) {
        const auto& name = table.table_name; 
        if(met_names.contains(name))
            return std::unexpected("Table \"" + name + "\" meets more than once in FROM clause.");
        met_names.insert(name);
    }
    return {};
}
CanBeValidated<void>::TypeOrError SelectQuery::check_all_tables_specified(const SelectorGenerators& generators, const TablesContainer& tables) {
    std::unordered_set<TableNameType> tables_used;
    for(const auto& generator: generators) {
        tables_used.merge(generator->get_used_tables());
    }
    std::for_each(tables.begin(), tables.end(), [&] (const auto& table) {
        tables_used.erase(table.table_name);
    });
    if(!tables_used.empty()) {
        StringType error = "Used tables { ";
        for(auto it = tables_used.begin(); it != tables_used.end(); ++it) {
            const auto& table = *it;
            error += table;
            if(std::next(it) != tables_used.end())
                error += ", ";
        }
        error += " } are not specified in FROM clause.";
        return std::unexpected(error);
    }
    return {};
}

CanBeValidated<void>::TypeOrError SelectQuery::check_if_from_exists_when_required(const SelectorGenerators& generators, const TablesContainer& tables) {
    if(!tables.empty()) return {};
    for(const auto& generator : generators) {
        if(generator->requires_from_clause()) {
            return std::unexpected("SELECT * query is invalid; FROM clause required.");
        }
    }
    return {};
}

SelectQuery::ExpectedQueryResult SelectQuery::resolve(const TableValueGathererFactory& gatherer_factory) {
    bool has_empty_table = false;
    auto gatherers = create_gatherers(gatherer_factory, has_empty_table);
    if(!gatherers) 
        return std::unexpected(gatherers.error());
    auto columns = generate_columns();
    if(!columns)
        return std::unexpected(columns.error());
    ResultTable result_table = create_result_table_header(*columns);
    if(has_empty_table) 
        return std::make_shared<TableQueryResult>(std::move(result_table));
    auto [gatherers_hash, gatherers_ordered] = std::move(*gatherers);
    do {
        auto row = resolve_row(gatherers_hash, *columns);
        if(!row) return std::unexpected(row.error());
        result_table.push_back(*row);
    } while(jump_to_next_row(gatherers_ordered));
    return std::make_shared<TableQueryResult>(std::move(result_table));
}

ExpectedOrStr<SelectQuery::Selectors> SelectQuery::generate_columns() {
    Selectors columns;
    for(const auto& gen : selector_generators_) {
        auto selectors = gen->generate(tables_);
        if(!selectors)
            return std::unexpected(selectors.error());
        columns.splice(columns.end(), *selectors); 
    }
    return columns;
}

ResultTable SelectQuery::create_result_table_header(const Selectors& columns) const {
    ResultTable result_table(1);
    for(const Selector& column : columns) { 
        result_table[0].push_back(column.column_name);
    }
    return result_table;
}

ExpectedOrStr<SelectQuery::GatherersTuple> SelectQuery::create_gatherers(const TableValueGathererFactory& gatherer_factory, bool& has_empty_table) const {
    has_empty_table = false;
    TablesGathered gatherers_hash {};
    OrderedGatherers gatherers_ordered {};
    for(const Table& selected_table : tables_) {
        auto exp_gatherer = gatherer_factory.build_cell_value_gatherer(selected_table.table_name);
        if(!exp_gatherer)
            return std::unexpected(exp_gatherer.error());
        if((*exp_gatherer)->is_table_empty()) {
            has_empty_table = true;
            return std::tuple{ gatherers_hash, gatherers_ordered };
        }
        gatherers_hash[selected_table.table_name] = *exp_gatherer;
        gatherers_ordered.push_back(*exp_gatherer);
    }
    return std::tuple(gatherers_hash, gatherers_ordered);
}

bool SelectQuery::jump_to_next_row(OrderedGatherers& gatherers) {
    auto iter = gatherers.rbegin();
    while(iter != gatherers.rend() && (*iter)->jump_to_next_row()) {
        ++iter;
    }
    return iter != gatherers.rend();
}

ExpectedOrStr<ResultRow> SelectQuery::resolve_row(const TablesGathered& gatherers, const Selectors& columns) const {
    ResultRow result_row; result_row.reserve(columns.size());
    for(const Selector& column : columns) {
        auto resolved = resolve_and_stringfy(column, gatherers);
        if(!resolved) return std::unexpected(resolved.error());
        result_row.push_back(*resolved);
    }
    return result_row;
}

ExpectedOrStr<StringType> SelectQuery::resolve_and_stringfy(const Selector& column, const TablesGathered& gatherers) const {
    std::stringstream ss;
    auto result = column.ast->resolve(gatherers);
    if(!result)
        return std::unexpected(result.error());
    (*result)->format(ss);
    return ss.str();
}

}
