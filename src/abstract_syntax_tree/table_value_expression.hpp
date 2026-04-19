#pragma once
#include "expression.hpp"

namespace garlic {

class TableValueExpression : public Expression {
public:
    TableValueExpression(std::string column_name, CellType type);

    ExpectedCellValue resolve(sptr<TableValueGatherer> gatherer) const override;
    
    ExpectedValid validate() const override;
    
private:
    std::string column_name_;
};

}
