#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellAcceptMathOp : public CellValue {
protected:
    CellAcceptMathOp(CellType ct)
    : CellValue{ ct }
    {}
public:
    virtual ~CellAcceptMathOp() = default;

    virtual CellValuePtr add(CellValuePtr other) const = 0;
    virtual CellValuePtr sub(CellValuePtr other) const = 0;
    virtual CellValuePtr mul(CellValuePtr other) const = 0;
    virtual CellValuePtr div(CellValuePtr other) const = 0;
    
};

}
