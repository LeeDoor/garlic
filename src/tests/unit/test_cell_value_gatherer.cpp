#include "cell_comparable.hpp"
#include "cell_float_value.hpp"
#include "cell_int_value.hpp"
#include "cell_string_view_value.hpp"
#include "cell_value_gatherer_impl.hpp"
#include "public_column_info.hpp"
#include "typed_table.hpp"

namespace garlic::tests {

using namespace garlic::table;

using namespace garlic::abstract_syntax_tree;

class CellValueGathererFixture : public ::testing::Test {
  protected:
  public:
	CellValueGathererFixture() : table_{initialize_table()} {}

  protected:
	sptr<TypedTable> initialize_table() {
		auto table = std::make_shared<TypedTable>(std::initializer_list<PublicColumnInfo>{
		    {String, "String field", 10}, {Float, "Float field", 0}, {Int, "Int field", 0}});

		table->create_empty_row();
		table->create_empty_row();

		table->set_value(0, 0, str_Aboba10_);
		table->set_value(0, 1, 1.6f);
		table->set_value(0, 2, 1);

		table->set_value(1, 0, str_TEST2026_);
		table->set_value(1, 1, 1e10f + 5);
		table->set_value(1, 2, INT_MAX - 2024);

		return table;
	}

#define STR(name)                                                                                                      \
	std::string str_##name##_ = #name;                                                                                 \
	std::string_view str_##name = str_##name##_;
	STR(Aboba10)
	STR(TEST2026)

	sptr<TypedTable> table_;
};

TEST_F(CellValueGathererFixture, init) {
	sptr<CellValueGathererImpl> tvg = std::make_shared<CellValueGathererImpl>(table_);
}

TEST_F(CellValueGathererFixture, accessingData_0ByDefault) {
	sptr<CellValueGathererImpl> tvg = std::make_shared<CellValueGathererImpl>(table_);

	SingleTableRow row = tvg->gather_single_row();
	auto cmp_str = std::dynamic_pointer_cast<CellComparable>(row["String field"]);
	auto cmp_float = std::dynamic_pointer_cast<CellComparable>(row["Float field"]);
	auto cmp_int = std::dynamic_pointer_cast<CellComparable>(row["Int field"]);
	ASSERT_NE(cmp_str, nullptr);
	ASSERT_NE(cmp_float, nullptr);
	ASSERT_NE(cmp_int, nullptr);

	EXPECT_TRUE(cmp_str->equals(std::make_shared<CellStringViewValue>(str_Aboba10)));
	EXPECT_TRUE(cmp_float->equals(std::make_shared<CellFloatValue>(1.6f)));
	EXPECT_TRUE(cmp_int->equals(std::make_shared<CellIntValue>(1)));
}

TEST_F(CellValueGathererFixture, accessingData_rowSelect) {
	sptr<CellValueGathererImpl> tvg = std::make_shared<CellValueGathererImpl>(table_);
	EXPECT_FALSE(tvg->jump_to_next_row());

	SingleTableRow row = tvg->gather_single_row();

	auto cmp_str = std::dynamic_pointer_cast<CellComparable>(row["String field"]);
	auto cmp_float = std::dynamic_pointer_cast<CellComparable>(row["Float field"]);
	auto cmp_int = std::dynamic_pointer_cast<CellComparable>(row["Int field"]);
	ASSERT_NE(cmp_str, nullptr);
	ASSERT_NE(cmp_float, nullptr);
	ASSERT_NE(cmp_int, nullptr);

	EXPECT_TRUE(cmp_str->equals(std::make_shared<CellStringViewValue>(str_TEST2026)));
	EXPECT_TRUE(cmp_float->equals(std::make_shared<CellFloatValue>(1e10f + 5)));
	EXPECT_TRUE(cmp_int->equals(std::make_shared<CellIntValue>(INT_MAX - 2024)));
}

TEST_F(CellValueGathererFixture, jumpToNextRow_afterLastRowShouldResetAndReturnTrue) {
	sptr<CellValueGathererImpl> tvg = std::make_shared<CellValueGathererImpl>(table_);
	EXPECT_FALSE(tvg->jump_to_next_row());
	EXPECT_TRUE(tvg->jump_to_next_row());

	SingleTableRow row = tvg->gather_single_row();

	auto cmp_str = std::dynamic_pointer_cast<CellComparable>(row["String field"]);
	auto cmp_float = std::dynamic_pointer_cast<CellComparable>(row["Float field"]);
	auto cmp_int = std::dynamic_pointer_cast<CellComparable>(row["Int field"]);
	ASSERT_NE(cmp_str, nullptr);
	ASSERT_NE(cmp_float, nullptr);
	ASSERT_NE(cmp_int, nullptr);

	EXPECT_TRUE(cmp_str->equals(std::make_shared<CellStringViewValue>(str_Aboba10)));
	EXPECT_TRUE(cmp_float->equals(std::make_shared<CellFloatValue>(1.6f)));
	EXPECT_TRUE(cmp_int->equals(std::make_shared<CellIntValue>(1)));
}

TEST_F(CellValueGathererFixture, gatheredRowContainsOnlyExistingColumns) {
	sptr<CellValueGathererImpl> tvg = std::make_shared<CellValueGathererImpl>(table_);

	SingleTableRow row = tvg->gather_single_row();

	EXPECT_EQ(row.size(), 3);
	EXPECT_TRUE(row.contains("String field"));
	EXPECT_TRUE(row.contains("Float field"));
	EXPECT_TRUE(row.contains("Int field"));
	EXPECT_FALSE(row.contains("sTRING FIELD"));
	EXPECT_FALSE(row.contains("afaejpffield"));
	EXPECT_FALSE(row.contains("Int field@@@"));
}

} // namespace garlic::tests
