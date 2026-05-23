#include "cell_value_gatherer_impl.hpp"
#include "cell_float_value.hpp"
#include "cell_int_value.hpp"
#include "cell_string_view_value.hpp"
#include "typed_table.hpp"

namespace garlic {

CellValueGathererImpl::CellValueGathererImpl(sptr<TypedTable> table)
    : table_{ table }
    , row_number_{ 0 }
{}

bool CellValueGathererImpl::is_table_empty() const {
    return table_->is_row_index_overflow(0);
}

sptr<CellValue> CellValueGathererImpl::get_table_value(const ColumnNameType& column_name) {
    auto column_number = table_->get_column_number_by_name(column_name);
    if(!column_number)
        throw std::logic_error("called invalid CellValueGathererImpl::get_table_value: no such column \"" + column_name + "\"");
    CellType type = table_->get_column_type(*column_number);
    switch(type) {
        case String:
            return std::make_shared<CellStringViewValue>(
                table_->get_value<StringType>(row_number_, *column_number)
            );
        case Int:
            return std::make_shared<CellIntValue>(
                table_->get_value<IntType>(row_number_, *column_number)
            );
        case Float:
            return std::make_shared<CellFloatValue>(
                table_->get_value<FloatType>(row_number_, *column_number)
            );
        default:
            std::unreachable();
    }
}

bool CellValueGathererImpl::jump_to_next_row() {
    ++row_number_;
    if(table_->is_row_index_overflow(row_number_)) {
        row_number_ = 0;
        return true;
    }
    return false;
}

}
