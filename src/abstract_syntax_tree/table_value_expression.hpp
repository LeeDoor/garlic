#pragma once
#include "expression.hpp"

namespace garlic {

class TableValueExpression : public Expression {
public:
    TableValueExpression(std::string column_name);

    sptr<CellValue> get_value(sptr<TableValueGatherer> gatherer) const override;

private:
    std::string column_name_;
};

}
