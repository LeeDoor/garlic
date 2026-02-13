#include "typed_table.hpp"

namespace garlic {

TEST(test_typed_table, initEmpty_shouldThrow) {
    EXPECT_THROW(TypedTable tt {}, std::logic_error);
}

TEST(test_typed_table, initWithColumns) {
    TypedTable tt = {
        { String, "Name", 10 },
        { Int, "Age", 0 },
        { Int, "height", 0 },
        { Float, "Weight", 0 },
    };
}

TEST(test_typed_table, specifyingTypesSize_shouldBeIgnored) {
    TypedTable tt = {
        { String, "Name", 10 },
        { Int, "Age", INT_MAX },
        { Int, "height", 1000 },
        { Float, "Weight", 999 },
    };
}

TEST(test_typed_table, initWithAlotofColumnsWithVector) {
    std::vector<PublicColumnInfo> pcis;
    pcis.reserve(1000);
    for(int i = 0; i < 1000; ++i) {
        pcis.push_back( { String, "Name" + std::to_string(i), 20 });
    }
    TypedTable tt(pcis);
}

TEST(test_typed_table, initWithColumns_preventSameColumnsName) {
    auto two_string_columns_same_name = [] {
        TypedTable tt {
            {String, "Name", 10 },
            {String, "Name", 20 }
        };
    };
    auto string_and_int_columns_same_name = [] {
        TypedTable tt {
            {String, "Name", 10 },
            {Int, "Name", 20 }
        };
    };

    ASSERT_THROW(two_string_columns_same_name(), std::logic_error);
    ASSERT_THROW(string_and_int_columns_same_name(), std::logic_error);
}

TEST(test_typed_table, creatingRows_shouldnotThrow) {
    TypedTable tt {
        { String, "Name", 10 }
    };
    for(int i = 0; i < 1000; ++i)
        EXPECT_NO_THROW(tt.create_empty_row());
}

TEST(test_typed_table, creatingManyRowsManyColumns_shouldnotThrow) {
    std::vector<PublicColumnInfo> pcis;
    pcis.reserve(1000);
    for(int i = 0; i < 1000; ++i) {
        pcis.push_back( { String, "Name" + std::to_string(i), 20 });
    }
    TypedTable tt(pcis);

    for(int i = 0; i < 1000; ++i)
        EXPECT_NO_THROW(tt.create_empty_row());
}

TEST(test_typed_table, initedTable_zerosByDefault) {
    TypedTable tt = {
        { String, "Name", 10 },
        { Int, "Age", 0 },
        { Float, "Weight", 0 },
    };
    tt.create_empty_row();
    tt.create_empty_row();

    for(int i = 0; i < 2; ++i) {
        EXPECT_RANGEQ(tt.get_value<StringType>(i, 0), std::string(0, '\0'));
        EXPECT_EQ(tt.get_value<IntType>(i, 1), 0);
        EXPECT_FLOAT_EQ(tt.get_value<FloatType>(i, 2), 0);
    }

}

TEST(test_typed_table, initedTable_insertingData) {
    TypedTable tt = {
        { String, "Name", 10 },
        { Int, "Age", 0 },
        { Float, "Weight", 0 },
    };
    tt.create_empty_row();

    tt.set_value(0, 0, "Sergey Jo");
    tt.set_value(0, 1, 28);
    tt.set_value(0, 2, 64.55f);

    EXPECT_EQ(tt.get_value<StringType>(0, 0), "Sergey Jo");
    EXPECT_EQ(tt.get_value<IntType>(0, 1), 28);
    EXPECT_FLOAT_EQ(tt.get_value<FloatType>(0, 2), 64.55);
}

TEST(test_typed_table, initedTable_insertingDataToDifferentRows) {
    TypedTable tt = {
        { String, "Name", 10 },
        { Int, "Age", 0 },
        { Float, "Weight", 0 },
    };
    tt.create_empty_row();
    tt.create_empty_row();

    tt.set_value(0, 0, "Sergey Jo");
    tt.set_value(0, 1, 28);
    tt.set_value(0, 2, 64.55f);
    tt.set_value(1, 0, "Oleg Ivan");
    tt.set_value(1, 1, 12);
    tt.set_value(1, 2, -574.100023f);

    EXPECT_EQ(tt.get_value<StringType>(0, 0), "Sergey Jo");
    EXPECT_EQ(tt.get_value<IntType>(0, 1), 28);
    EXPECT_FLOAT_EQ(tt.get_value<FloatType>(0, 2), 64.55f);

    EXPECT_EQ(tt.get_value<StringType>(1, 0), "Oleg Ivan");
    EXPECT_EQ(tt.get_value<IntType>(1, 1), 12);
    EXPECT_FLOAT_EQ(tt.get_value<FloatType>(1, 2), -574.100023f);
}

TEST(test_typed_table, initedTable_missStringToInt_throwsWithoutChanges) {
    TypedTable tt = {
        { String, "Name", 10 },
        { Int, "Age", 0 },
        { Float, "Weight", 0 },
    };
    tt.create_empty_row();
    tt.create_empty_row();

    tt.set_value(0, 0, "Sergey Jo");
    EXPECT_THROW(tt.set_value(0, 0, 1485), std::logic_error);

    EXPECT_EQ(tt.get_value<StringType>(0, 0), "Sergey Jo");
}

TEST(test_typed_table, initedTable_missIntToFloat_throwsWithoutChanges) {
    TypedTable tt = {
        { String, "Name", 10 },
        { Int, "Age", 0 },
        { Float, "Weight", 0 },
    };
    tt.create_empty_row();
    tt.create_empty_row();

    tt.set_value(0, 1, INT_MAX - 1);
    EXPECT_THROW(tt.set_value(0, 1, 2147.123151f), std::logic_error);

    EXPECT_EQ(tt.get_value<IntType>(0, 1), INT_MAX - 1);
}

TEST(test_typed_table, initedTable_ReadingWithWrongType_shouldThrow) {
    TypedTable tt = {
        { String, "Name", 10 },
        { Int, "Age", 0 },
        { Float, "Weight", 0 },
    };
    tt.create_empty_row();

    EXPECT_THROW(tt.get_value<StringType>(0, 1), std::logic_error);
    EXPECT_THROW(tt.get_value<StringType>(0, 2), std::logic_error);
    EXPECT_THROW(tt.get_value<IntType>(0, 0), std::logic_error);
    EXPECT_THROW(tt.get_value<IntType>(0, 2), std::logic_error);
    EXPECT_THROW(tt.get_value<FloatType>(0, 0), std::logic_error);
    EXPECT_THROW(tt.get_value<FloatType>(0, 1), std::logic_error);
}

TEST(test_typed_table, tableWithStrings_ReadWriteMoreCharactersThanGiven_ShouldThrow) {
    TypedTable tt = {
        { String, "Name", 10 },
        { String, "Surname", 150 },
    };
    tt.create_empty_row();

    EXPECT_NO_THROW(tt.set_value(0, 0, "1234567890"));
    EXPECT_THROW(tt.set_value(0, 0, "1234567890X"), std::logic_error);
    EXPECT_THROW(tt.set_value(0, 0, std::string(120, 'A')), std::logic_error);
    EXPECT_NO_THROW(tt.set_value(0, 1, "123456789"));
    EXPECT_NO_THROW(tt.set_value(0, 1, std::string(120, 'A')));
    EXPECT_THROW(tt.set_value(0, 1, std::string(170, 'A')), std::logic_error);
    EXPECT_THROW(tt.set_value(0, 1, std::string(151, 'A')), std::logic_error);
}

TEST(test_typed_table, getColumnNumberByName) {
    TypedTable tt = {
        { String, "Name", 10 },
        { String, "Surname", 150 },
        { Float, "height", 0 },
    };

    EXPECT_EQ(tt.get_column_number_by_name("Name"), 0);
    EXPECT_EQ(tt.get_column_number_by_name("Surname"), 1);
    EXPECT_THROW(tt.get_column_number_by_name("surname"), std::logic_error); // Case-sensitive
    EXPECT_EQ(tt.get_column_number_by_name("height"), 2);
}

TEST(test_typed_table, getColumnNumberByName_withRows) {
    TypedTable tt = {
        { String, "Name", 10 },
        { String, "Surname", 150 },
        { Float, "height", 0 },
    };

    tt.create_empty_row();
    tt.create_empty_row();

    EXPECT_EQ(tt.get_column_number_by_name("Name"), 0);
    EXPECT_EQ(tt.get_column_number_by_name("Surname"), 1);
    EXPECT_THROW(tt.get_column_number_by_name("surname"), std::logic_error); // Case-sensitive
    EXPECT_EQ(tt.get_column_number_by_name("height"), 2);
}

}
