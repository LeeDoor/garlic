#include "cell_value_gatherer_impl.hpp"
#include "cell_float_value.hpp"
#include "cell_int_value.hpp"
#include "cell_string_view_value.hpp"
#include "typed_table.hpp"

namespace garlic::abstract_syntax_tree {

using namespace garlic::table;

CellValueGathererImpl::CellValueGathererImpl(sptr<garlic::table::TypedTable> table) : table_{table}, row_number_{0} {}

bool CellValueGathererImpl::is_table_empty() const { return table_->is_row_index_overflow(0); }

SingleTableRow CellValueGathererImpl::gather_single_row() {
	SingleTableRow row;
	auto tables_header = table_->get_header();
	for (size_t column = 0; column < tables_header.size(); ++column) {
		CellType type = table_->get_column_type(column);
		sptr<CellValue> cell_value;
		switch (type) {
		case String:
			cell_value = std::make_shared<CellStringViewValue>(table_->get_value<StringType>(row_number_, column));
            break;
		case Int:
			cell_value = std::make_shared<CellIntValue>(table_->get_value<IntType>(row_number_, column));
            break;
		case Float:
			cell_value = std::make_shared<CellFloatValue>(table_->get_value<FloatType>(row_number_, column));
            break;
		default:
			std::unreachable();
		}
		auto column_name = tables_header[column].name;
		row.insert({column_name, cell_value});
	}
	return row;
}

bool CellValueGathererImpl::jump_to_next_row() {
	++row_number_;
	if (table_->is_row_index_overflow(row_number_)) {
		row_number_ = 0;
		return true;
	}
	return false;
}

} // namespace garlic::abstract_syntax_tree
