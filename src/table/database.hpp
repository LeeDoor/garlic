#pragma once
#include "cell_value_gatherer_impl.hpp"
#include "table_value_gatherer_factory_impl.hpp"
#include "tables_header_gatherer_impl.hpp"
#include "typed_table.hpp"
#include "table_value_gatherer.hpp"

namespace garlic {

/// Array of all tables in the database
template<typename TableGathererT>
    requires TableColumnTypeGatherer<TableGathererT>
    && TableValueGatherer<TableGathererT>
class DatabaseImpl {
private:
    using TablesContainer = std::unordered_map<TableNameType, sptr<TableGathererT>>;
public:
    DatabaseImpl(TablesContainer&& tables)
    : tables_{ std::move(tables) }
    {}

    ExpectedColumnType get_tables_column_type(const TableNameType& table_name, const ColumnNameType& column_name) const {
	auto table = get_table_by_name(table_name);
	if(!table) return std::unexpected(table.error());
	return (*table)->get_column_type(column_name);
    }
    inline ExpectedCellValueGatherer build_cell_value_gatherer(const TableNameType& table_name) const {
	return build_cell_value_gatherer_impl(*this, table_name);
    }
    ExpectedCellValueGatherer build_cell_value_gatherer(const TableNameType& table_name) {
	return build_cell_value_gatherer_impl(*this, table_name);
    }

private:
    template<typename T>
    static ExpectedCellValueGatherer 
	build_cell_value_gatherer_impl(T& self, const TableNameType& table_name) {
	auto table = self.get_table_by_name(table_name);
	if(!table) return std::unexpected(table.error());
	return std::make_shared<CellValueGathererImpl>(*table);
    }
    std::expected<sptr<TableGathererT>, StringType> get_table_by_name(const TableNameType& table_name) const {
	if(!tables_.contains(table_name))
	    return std::unexpected("Table " + table_name + " does not exist.");
	return tables_.at(table_name);
    }
    TablesContainer tables_;
};

using Database = DatabaseImpl<TypedTable>;

static_assert(TablesHeaderGathererImpl<Database>);
static_assert(TableValueGathererFactoryImpl<Database>);

}
