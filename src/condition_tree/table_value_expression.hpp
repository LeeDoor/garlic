#pragma once
#include "expression.hpp"

namespace garlic {

class TableValueExpression : public Expression {
public:
    TableValueExpression(CellType type, std::string column_name)
    : Expression( type )
    , column_name_{ column_name }
    {} 

    CellValuePtr get_value(TableValueGathererPtr gatherer) const override {
        return gatherer->get_table_value(column_name_);
    }

private:
    std::string column_name_;
};

}
