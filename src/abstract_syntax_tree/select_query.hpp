#pragma once
#include "query.hpp"
#include "result_table.hpp"
#include "selector.hpp"
#include "selector_generator.hpp"
#include "table.hpp"

namespace garlic {

class SelectQuery : public Query {
public:
    // #TODO rename: remove Container suffix and Selectors -> Selectors
    using SelectorGeneratorsContainer = std::list<uptr<SelectorGenerator>>;
    using Selectors = std::list<Selector>;
    using TablesContainer = std::list<Table>;

    SelectQuery();
    SelectQuery(SelectorGeneratorsContainer selector_gens);
    SelectQuery(SelectorGeneratorsContainer selector_gens, TablesContainer tables);

    ExpectedQueryResult resolve(const TableValueGathererFactory& gatherer_factory) override;

private:
    using OrderedGatherers = std::list<sptr<CellValueGatherer>>;
    using GatherersTuple = std::tuple<TablesGathered, OrderedGatherers>;

    static CanBeValidated<void>::TypeOrError is_valid(const SelectorGeneratorsContainer& , const TablesContainer& );
    static CanBeValidated<void>::TypeOrError check_all_tables_specified(const SelectorGeneratorsContainer& , const TablesContainer& );
    static CanBeValidated<void>::TypeOrError check_no_same_tables(const TablesContainer& );
    static CanBeValidated<void>::TypeOrError check_if_from_exists_when_required(const SelectorGeneratorsContainer& , const TablesContainer& );

    ExpectedOrStr<Selectors> generate_columns();
    ResultTable create_result_table_header(const Selectors& ) const;
    ExpectedOrStr<GatherersTuple> create_gatherers(const TableValueGathererFactory& gatherer_factory, bool& has_empty_table) const;
    ExpectedOrStr<StringType> resolve_and_stringfy(const Selector& column, const TablesGathered& gatherers) const;
    ExpectedOrStr<ResultRow> resolve_row(const TablesGathered& gatherers, const Selectors& columns) const;
    static bool jump_to_next_row(OrderedGatherers& gatherers);

    TablesContainer tables_ {};
    SelectorGeneratorsContainer selector_generators_{};
};

}
