#pragma once
#include "expression.hpp"

namespace garlic {

class TableValueExpression : public Expression {
public:
    TableValueExpression(std::string column_name);

    CellValuePtr get_value(TableValueGathererPtr gatherer) const override;

private:
    std::string column_name_;
};

}
