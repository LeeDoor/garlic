#pragma once
#include "expression.hpp"

namespace garlic {

class TableValueExpression : public Expression {
public:
    TableValueExpression(std::string column_name, CellType type);

    ExpectedCellValue get_value(sptr<TableValueGatherer> gatherer) const override;
    
    std::optional<StringType> validate() const override;
    
private:
    std::string column_name_;
};

}
