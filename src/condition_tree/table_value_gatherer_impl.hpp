#pragma once
#include "table_value_gatherer.hpp"
#include "typed_table.hpp"

namespace garlic {

class TableValueGathererImpl : public TableValueGatherer {
public:
    TableValueGathererImpl(sptr<TypedTable> table);

    sptr<CellValue> get_table_value(const std::string& column_name) override;
    void set_row_number(size_t row_number);

protected:
    sptr<TypedTable> table_;
    size_t row_number_;
};

}
