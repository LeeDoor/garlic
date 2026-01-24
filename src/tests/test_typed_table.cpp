#include "typed_table.hpp"

namespace garlic {

TEST(test_typed_table, initialization) {
    EXPECT_NO_THROW(TypedTable tt {});
}

TEST(test_typed_table, initWithColumns) {
    TypedTable tt = {
        { String, "Name", 10 },
        { Int, "Age", 0 },
        { Float, "Weight", 0 },
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
        TypedTable tt1 {
            {String, "Name", 10 },
            {String, "Name", 20 }
        };
    };
    auto string_and_int_columns_same_name = [] {
        TypedTable tt2 {
            {String, "Name", 10 },
            {Int, "Name", 20 }
        };
    };

    ASSERT_THROW(two_string_columns_same_name(), std::logic_error);
    ASSERT_THROW(string_and_int_columns_same_name(), std::logic_error);
}

}
