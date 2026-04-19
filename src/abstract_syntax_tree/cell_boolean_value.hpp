#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellBooleanValue : public CellValue {
public:
    CellBooleanValue(bool bool_value);

    void format(std::ostream& os) const override;

    explicit operator bool() const;
    bool get_bool() const;
    
    bool conjunction(sptr<CellBooleanValue> other) const;
    bool disjunction(sptr<CellBooleanValue> other) const;
    bool equivalence(sptr<CellBooleanValue> other) const;
    bool implication(sptr<CellBooleanValue> other) const;
    bool exclusiveor(sptr<CellBooleanValue> other) const;

protected:
    bool value_;
};

}

