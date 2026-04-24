#pragma once
#include "expression.hpp"
#include "tables_gatherer.hpp"

namespace garlic {

class TableValueExpression : public Expression {
public:
    template<TablesGatherer TablesGathererType>
    TableValueExpression(const TablesGathererType& tables_gatherer, const TableNameType& table_name, const ColumnNameType& column_name)
    : Expression{ tables_gatherer.get_tables_column_type(table_name, column_name) }
    , 
    , table_name_{ table_name }
    , column_name_{ column_name }
    {}

    ExpectedCellValue resolve(sptr<TableValueGatherer> gatherer) const override;
    
private:

    TableNameType table_name_;
    ColumnNameType column_name_;
};

}
