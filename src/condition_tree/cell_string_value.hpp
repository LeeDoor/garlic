#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellStringValue : public CellValue {
public:
    CellStringValue(StringViewType value);

    explicit operator StringViewType() const;

    StringViewType get_string() const;
    bool equals(CellValuePtr other) const override;
    bool le(CellValuePtr other) const override;
    bool lt(CellValuePtr other) const override;
    bool ge(CellValuePtr other) const override;
    bool gt(CellValuePtr other) const override;
    void format(std::ostream& os) const override;

private:
    int get_cmp(CellValuePtr other) const;

protected:
    StringViewType value_;
};

}
