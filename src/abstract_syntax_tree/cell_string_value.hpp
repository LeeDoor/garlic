#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellStringValue : public CellValue {
public:
    CellStringValue(StringViewType value);

    explicit operator StringViewType() const;

    StringViewType get_string() const;
    bool equals(sptr<CellValue> other) const override;
    bool le(sptr<CellValue> other) const override;
    bool lt(sptr<CellValue> other) const override;
    bool ge(sptr<CellValue> other) const override;
    bool gt(sptr<CellValue> other) const override;
    void format(std::ostream& os) const override;

private:
    int get_cmp(sptr<CellValue> other) const;

protected:
    StringViewType value_;
};

}
