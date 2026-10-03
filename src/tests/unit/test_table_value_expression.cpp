#include "cell_int_value.hpp"
#include "table_value_expression.hpp"
#include "tables_gatherer_mock.hpp"

namespace garlic::tests {

using namespace garlic::abstract_syntax_tree;

class TestTableValueExpressionFixture : public ::testing::Test {
  protected:
	TestTableValueExpressionFixture() {
		multiple_tables_row_.insert({"Table name", SingleTableRow{{"Column name", test_int_value_}}});
	}

	sptr<CellIntValue> test_int_value_{std::make_shared<CellIntValue>(5)};
	MultipleTablesRow multiple_tables_row_{};
};

TEST_F(TestTableValueExpressionFixture, createdWithCorrectArguments_resolveWithCorrectRow_shouldReturnCorrectCellPtr) {
	TableValueExpression expr{TablesGathererMock{Int}, "Table name", "Column name"};
	auto value = expr.resolve(multiple_tables_row_);

	ASSERT_TRUE(value.has_value()) << value.error();
	auto comparable_value = std::dynamic_pointer_cast<CellComparable>(*value);
	EXPECT_TRUE(comparable_value);
	EXPECT_TRUE(comparable_value->equals(test_int_value_));
}

TEST_F(TestTableValueExpressionFixture, createdWithMismatchingTableName_resolve_shouldReturnUnexpected) {
	TableValueExpression expr{TablesGathererMock{Int}, "Mismatching table name", "Column name"};

	EXPECT_THROW(std::ignore = expr.resolve(multiple_tables_row_), std::logic_error);
}

TEST_F(TestTableValueExpressionFixture, createdWithMismatchingColumnName_resolve_shouldReturnUnexpected) {
	TableValueExpression expr{TablesGathererMock{Int}, "Table name", "Mismatching column name"};

	EXPECT_THROW(std::ignore = expr.resolve(multiple_tables_row_), std::logic_error);
}

TEST_F(TestTableValueExpressionFixture, getUsedTables_ReturnsReferencedTable) {
	TableValueExpression expr{TablesGathererMock{Int}, "Table name", "Column name"};
	EXPECT_EQ(expr.get_used_tables(), TableValueExpression::UsedTables({"Table name"}));
}

} // namespace garlic::tests
