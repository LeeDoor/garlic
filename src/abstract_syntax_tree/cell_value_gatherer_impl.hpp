#pragma once
#include "cell_value_gatherer.hpp"

namespace garlic {
class TypedTable;

class CellValueGathererImpl : public CellValueGatherer {
public:
    CellValueGathererImpl(sptr<TypedTable> table);

    sptr<CellValue> get_table_value(const ColumnNameType& column_name) override;
    void set_row_number(size_t row_number);

protected:
    sptr<TypedTable> table_;
    size_t row_number_;
};

}
