#pragma once
#include "query.hpp"
#include "expression.hpp"
#include "result_table.hpp"

namespace garlic {

struct Selector { 
    ColumnNameType column_name;
    uptr<Expression> ast;

    Selector() : column_name{ "" }, ast{ nullptr } {}
    explicit Selector(uptr<Expression> expression)
    : ast{ std::move(expression) }
    {
	std::stringstream ss; TypeRules::as_str(ss, ast->get_type());
	column_name = ss.str();
    }
    Selector(const ColumnNameType& column_name, uptr<Expression> expression)
    : column_name{ column_name }
    , ast{ std::move(expression) }
    {}
};

struct Table {
    TableNameType table_name;
};

class SelectQuery : public Query {
public:
    using ColumnsContainer = std::list<Selector>;
    using TablesContainer = std::list<Table>;

    SelectQuery();
    SelectQuery(ColumnsContainer columns);
    SelectQuery(ColumnsContainer columns, TablesContainer tables);

    ExpectedQueryResult resolve(const TableValueGathererFactory& gatherer_factory) override;

private:
    using OrderedGatherers = std::list<sptr<CellValueGatherer>>;

    static CanBeValidated<void>::TypeOrError is_valid(const ColumnsContainer& columns, const TablesContainer& tables);
    static CanBeValidated<void>::TypeOrError check_all_tables_specified(const ColumnsContainer& columns, const TablesContainer& tables);
    static CanBeValidated<void>::TypeOrError check_no_same_tables(const TablesContainer& tables);

    ResultTable create_result_table_header() const;
    std::expected<std::pair<TablesGathered, OrderedGatherers>, UnexpectedCellValue> create_gatherers(const TableValueGathererFactory& gatherer_factory, bool& has_empty_table) const;
    std::expected<StringType, UnexpectedCellValue> resolve_and_stringfy(const Selector& column, const TablesGathered& gatherers) const;
    std::expected<ResultRow, UnexpectedCellValue> resolve_row(const TablesGathered& gatherers) const;
    static bool jump_to_next_row(OrderedGatherers& gatherers);

    ColumnsContainer columns_ {};
    TablesContainer tables_ {};
};

}
