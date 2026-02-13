#pragma once
#include "cell_float_value.hpp"
#include "cell_int_value.hpp"
#include "cell_string_value.hpp"
#include "table_value_gatherer.hpp"
#include "typed_table.hpp"

namespace garlic {

class TableValueGathererImpl : public TableValueGatherer {
public:
    TableValueGathererImpl(std::shared_ptr<TypedTable> table)
    : table_{ table }
    , row_number_{ 0 }
    {}

    CellValuePtr get_table_value(const std::string& column_name) override {
        size_t column_number = table_->get_column_number_by_name(column_name);
        CellType type = table_->get_column_type(column_number);
        switch(type) {
        case String:
            return std::make_shared<CellStringValue>(
                table_->get_value<StringType>(row_number_, column_number)
            );
        case Int:
            return std::make_shared<CellIntValue>(
                table_->get_value<IntType>(row_number_, column_number)
            );
        case Float:
            return std::make_shared<CellFloatValue>(
                table_->get_value<FloatType>(row_number_, column_number)
            );
        default:
            throw std::logic_error("get_table_value value type not implemented");
        }
    }

    void set_row_number(size_t row_number) {
        row_number_ = row_number;
    }

protected:
    std::shared_ptr<TypedTable> table_;
    size_t row_number_;
};

}
