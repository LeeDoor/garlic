#pragma once
#include "table_value_gatherer.hpp"
#include "typed_table.hpp"

namespace garlic {

class TableValueGathererImpl : public TableValueGatherer {
public:
    TableValueGathererImpl(std::shared_ptr<TypedTable> table);

    CellValuePtr get_table_value(const std::string& column_name) override;
    void set_row_number(size_t row_number);

protected:
    std::shared_ptr<TypedTable> table_;
    size_t row_number_;
};

}
