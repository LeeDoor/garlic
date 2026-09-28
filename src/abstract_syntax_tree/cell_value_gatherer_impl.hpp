#pragma once
#include "cell_value_gatherer.hpp"

namespace garlic::table {
class TypedTable;
}

namespace garlic::abstract_syntax_tree {

using namespace garlic::table;

class CellValueGathererImpl : public CellValueGatherer {
  public:
	CellValueGathererImpl(sptr<garlic::table::TypedTable> table);

	bool is_table_empty() const override;
	sptr<CellValue> get_table_value(const ColumnNameType &column_name) override;
	bool jump_to_next_row() override;

  protected:
	sptr<garlic::table::TypedTable> table_;
	size_t row_number_;
};

} // namespace garlic::abstract_syntax_tree
