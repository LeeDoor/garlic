#include "cell_string_value.hpp"
#include "cell_string_view_value.hpp"

namespace garlic {
using namespace std::literals;

TEST(test_cell_string_value, movedStringShouldBeStoredAndViewBound) {
    StringType src = "hello_world";
    CellStringValue value(std::move(src));

    EXPECT_EQ(value.get_type(), CellType::String);
    EXPECT_EQ(value.get_string_view(), "hello_world");
}

TEST(test_cell_string_value, temporaryStringShouldBeSafeAfterConstruction) {
    CellStringValue value(StringType("temporary_literal"));
    EXPECT_EQ(value.get_string_view(), "temporary_literal");
}

TEST(test_cell_string_value, delegatedComparisonOperationsShouldWork) {
    sptr<CellValue> lhs = std::make_shared<CellStringValue>(StringType("abc"));
    sptr<CellValue> rhs = std::make_shared<CellStringViewValue>("abd"sv);

    EXPECT_FALSE(lhs->equals(rhs));
    EXPECT_TRUE(lhs->lt(rhs));
    EXPECT_TRUE(lhs->le(rhs));
    EXPECT_FALSE(lhs->gt(rhs));
    EXPECT_FALSE(lhs->ge(rhs));
}

TEST(test_cell_string_value, movedFromSourceMutationShouldNotAffectStoredView) {
    StringType src = "stable_data";
    CellStringValue value(std::move(src));
    src = "changed";

    EXPECT_EQ(value.get_string_view(), "stable_data");
}

TEST(test_cell_string_value, stressManyLargeStringsShouldKeepValidViews) {
    std::vector<CellStringValue> values;
    values.reserve(2000);

    for(int i = 0; i < 2000; ++i) {
        StringType payload(1024, static_cast<char>('a' + (i % 26)));
        payload[0] = 'B';
        payload[1] = 'E';
        payload[2] = 'G';
        values.emplace_back(std::move(payload));
    }

    for(const auto& value : values) {
        auto sv = value.get_string_view();
        ASSERT_EQ(sv.size(), 1024);
        EXPECT_EQ(sv.substr(0, 3), "BEG");
    }
}

}
