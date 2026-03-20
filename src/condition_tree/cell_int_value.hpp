#pragma once
#include "cell_accept_math_op.hpp"
#include "get_cell_type.hpp"

namespace garlic {

class CellFloatValue;

class CellIntValue : public CellAcceptMathOp {
public:
    CellIntValue(IntType value);

    IntType operator
    IntType get_int() const;
    FloatType get_float() const;

    bool equals(CellValuePtr other) const override;
    bool le(CellValuePtr other) const override;
    bool lt(CellValuePtr other) const override;
    bool ge(CellValuePtr other) const override;
    bool gt(CellValuePtr other) const override;
    void format(std::ostream& os) const override;

    CellValuePtr add(CellValuePtr other) const override;
    CellValuePtr sub(CellValuePtr other) const override;
    CellValuePtr mul(CellValuePtr other) const override;
    CellValuePtr div(CellValuePtr other) const override;

private:
    template<typename CellValueType> 
    requires std::is_base_of_v<CellValue, CellValueType>
    static bool is_type(CellValuePtr other) {
        return std::dynamic_pointer_cast<CellValueType>(other) != nullptr;
    }
    template<IsAnyColumnType T> 
    static T to_type_ptr(CellValuePtr other) {
        auto ptr = std::dynamic_pointer_cast<get_cell_type<T>::Type>(other);
    }
    static bool is_int(CellValuePtr other);
    static IntType to_int(CellValuePtr other);
    static bool is_float(CellValuePtr other);
    static FloatType to_float(CellValuePtr other);
    std::unique_ptr<CellFloatValue> as_float() const;

protected:
    IntType value_;
};

}
