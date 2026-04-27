#pragma once
#include "cell_value_gatherer.hpp"

namespace garlic {
class TypedTable;

class CellValueGathererImpl : public CellValueGatherer {
public:
    CellValueGathererImpl(sptr<TypedTable> table);

    bool is_table_empty() const override;
    sptr<CellValue> get_table_value(const ColumnNameType& column_name) override;
    bool jump_to_next_row() override;

protected:
    sptr<TypedTable> table_;
    size_t row_number_;
};

}
