#pragma once
#include "query.hpp"
#include "result_table.hpp"
#include "selector.hpp"
#include "selector_generator.hpp"
#include "table.hpp"

namespace garlic {

class SelectQuery : public Query {
public:
    // #TODO rename: remove Container suffix and ColumnsContainer -> Selectors
    using SelectorGeneratorsContainer = std::list<uptr<SelectorGenerator>>;
    using ColumnsContainer = std::list<Selector>;
    using TablesContainer = std::list<Table>;

    SelectQuery();
    SelectQuery(SelectorGeneratorsContainer selector_gens);
    SelectQuery(SelectorGeneratorsContainer selector_gens, TablesContainer tables);

    ExpectedQueryResult resolve(const TableValueGathererFactory& gatherer_factory) override;

private:
    using OrderedGatherers = std::list<sptr<CellValueGatherer>>;

    static CanBeValidated<void>::TypeOrError is_valid(const SelectorGeneratorsContainer& , const TablesContainer& );
    static CanBeValidated<void>::TypeOrError check_all_tables_specified(const SelectorGeneratorsContainer& , const TablesContainer& );
    static CanBeValidated<void>::TypeOrError check_no_same_tables(const TablesContainer& );
    static CanBeValidated<void>::TypeOrError check_if_from_exists_when_required(const SelectorGeneratorsContainer& , const TablesContainer& );

    std::expected<ColumnsContainer, StringType> generate_columns();
    ResultTable create_result_table_header(const ColumnsContainer& ) const;
    std::expected<std::pair<TablesGathered, OrderedGatherers>, UnexpectedCellValue> create_gatherers(const TableValueGathererFactory& gatherer_factory, bool& has_empty_table) const;
    std::expected<StringType, UnexpectedCellValue> resolve_and_stringfy(const Selector& column, const TablesGathered& gatherers) const;
    std::expected<ResultRow, UnexpectedCellValue> resolve_row(const TablesGathered& gatherers, const ColumnsContainer& columns) const;
    static bool jump_to_next_row(OrderedGatherers& gatherers);

    TablesContainer tables_ {};
    SelectorGeneratorsContainer selector_generators_{};
};

}
