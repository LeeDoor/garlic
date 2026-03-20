#include "table_value_gatherer_impl.hpp"
#include "cell_string_value.hpp"
#include "cell_float_value.hpp"
#include "cell_int_value.hpp"

namespace garlic {
class TableValueGathererFixture : public ::testing::Test {
protected:


public:
    TableValueGathererFixture()
        : table_{ initialize_table() }
    {}

protected:
    sptr<TypedTable> initialize_table() {
        auto table = std::make_shared<TypedTable>(
            std::initializer_list<PublicColumnInfo>{
            { String, "String field", 10 },
            { Float,  "Float field",  0  },
            { Int,    "Int field",    0  }
        });
        
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
    
#define STR(name) std::string str_##name##_ = #name; std::string_view str_##name = str_##name##_;
    STR(Aboba10)
    STR(TEST2026)

    sptr<TypedTable> table_;
};


TEST_F(TableValueGathererFixture, init) {
    sptr<TableValueGathererImpl> tvg = std::make_shared<TableValueGathererImpl>(table_);
}

TEST_F(TableValueGathererFixture, accessingData_0ByDefault) {
    sptr<TableValueGathererImpl> tvg = std::make_shared<TableValueGathererImpl>(table_);

    sptr<CellValue> cellstr = tvg->get_table_value("String field");
    sptr<CellValue> cellfloat = tvg->get_table_value("Float field");
    sptr<CellValue> cellint = tvg->get_table_value("Int field");

    EXPECT_TRUE(cellstr->equals(std::make_shared<CellStringValue>(str_Aboba10)));
    EXPECT_TRUE(cellfloat->equals(std::make_shared<CellFloatValue>(1.6f)));
    EXPECT_TRUE(cellint->equals(std::make_shared<CellIntValue>(1)));
}

TEST_F(TableValueGathererFixture, accessingData_rowSelect) {
    sptr<TableValueGathererImpl> tvg = std::make_shared<TableValueGathererImpl>(table_);
    tvg->set_row_number(1);

    sptr<CellValue> cellstr = tvg->get_table_value("String field");
    sptr<CellValue> cellfloat = tvg->get_table_value("Float field");
    sptr<CellValue> cellint = tvg->get_table_value("Int field");

    EXPECT_TRUE(cellstr->equals(std::make_shared<CellStringValue>(str_TEST2026)));
    EXPECT_TRUE(cellfloat->equals(std::make_shared<CellFloatValue>(1e10f + 5)));
    EXPECT_TRUE(cellint->equals(std::make_shared<CellIntValue>(INT_MAX - 2024)));
}

TEST_F(TableValueGathererFixture, wrongRowNumber_shouldThrowLogic) {
    sptr<TableValueGathererImpl> tvg = std::make_shared<TableValueGathererImpl>(table_);
    tvg->set_row_number(3);

    EXPECT_THROW(tvg->get_table_value("String field"), std::logic_error);
    EXPECT_THROW(tvg->get_table_value("Float field"), std::logic_error);
    EXPECT_THROW(tvg->get_table_value("Int field"), std::logic_error);
}

TEST_F(TableValueGathererFixture, misspellColumnName_throwLogic) {
    sptr<TableValueGathererImpl> tvg = std::make_shared<TableValueGathererImpl>(table_);
    tvg->set_row_number(0);

    EXPECT_THROW(tvg->get_table_value("sTRING FIELD"), std::logic_error);
    EXPECT_THROW(tvg->get_table_value("afaejpffield"), std::logic_error);
    EXPECT_THROW(tvg->get_table_value("Int field@@@"), std::logic_error);
}

}
