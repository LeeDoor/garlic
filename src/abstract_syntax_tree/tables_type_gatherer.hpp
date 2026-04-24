#pragma once
#include "table_value_expression.hpp"
#include "tables_gatherer.hpp"

namespace garlic {

template<TablesGatherer TablesGathererType>
class TablesTypeGatherer {
public:
    using UnexpectedTableValue = StringType;
    using ExpectedTableValueExpression = std::expected<uptr<TableValueExpression>, UnexpectedTableValue>;

    TablesTypeGatherer(TablesGathererType&& tables_gatherer)
    : tables_gatherer_{ std::move(tables_gatherer) }
    {}

    ExpectedTableValueExpression get_table_value_expression(
	    const TableNameType& table_name, 
	    const ColumnNameType& column_name) {
	auto type_or_err = tables_gatherer_.get_tables_column_type(table_name, column_name);
	if(!type_or_err) 
	    return std::unexpected(type_or_err.error());
	return std::make_unique<TableValueExpression>(table_name, column_name, *type_or_err);
    }
private:
    TablesGathererType tables_gatherer_;
};

}
