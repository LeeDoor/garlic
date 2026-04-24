#include "table_value_gatherer_impl.hpp"
#include "cell_float_value.hpp"
#include "cell_int_value.hpp"
#include "cell_string_view_value.hpp"
#include "typed_table.hpp"

namespace garlic {

TableValueGathererImpl::TableValueGathererImpl(sptr<TypedTable> table)
: table_{ table }
, row_number_{ 0 }
{}

sptr<CellValue> TableValueGathererImpl::get_table_value(const ColumnNameType& column_name) {
    size_t column_number = table_->get_column_number_by_name(column_name);
    CellType type = table_->get_column_type(column_number);
    switch(type) {
    case String:
	return std::make_shared<CellStringViewValue>(
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

void TableValueGathererImpl::set_row_number(size_t row_number) {
    row_number_ = row_number;
}

}
