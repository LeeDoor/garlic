#pragma once
#include "cell_value.hpp"

namespace garlic {

class CellStringViewValue : public CellValue {
public:
    CellStringViewValue(StringViewType value);
    virtual ~CellStringViewValue() = default;

    explicit operator StringViewType() const;

    StringViewType get_string_view() const;
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
