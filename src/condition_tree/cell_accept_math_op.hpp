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

    virtual sptr<CellValue> add(sptr<CellValue> other) const = 0;
    virtual sptr<CellValue> sub(sptr<CellValue> other) const = 0;
    virtual sptr<CellValue> mul(sptr<CellValue> other) const = 0;
    virtual sptr<CellValue> div(sptr<CellValue> other) const = 0;
    
};

}
