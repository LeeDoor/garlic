#include "byte_matrix.hpp"

bool operator== (const garlic::ByteSpan& lhs, const garlic::ByteVector& rhs) {
    return std::ranges::equal(lhs, rhs);
}

namespace garlic {

TEST(test_byte_matrix, empty_initialization) {
    ByteMatrix tc(0);

    ASSERT_THROW(tc.set_value(0, 0, ByteVector{}), std::logic_error);
    ASSERT_THROW(tc.set_value(1, 0, ByteVector{}), std::logic_error);
    ASSERT_THROW(tc.set_value(0, 1, ByteVector{}), std::logic_error);
    ASSERT_THROW(tc.set_value(0, 0, {0xDE, 0xAD, 0xBE, 0xEF}), std::logic_error);
    ASSERT_THROW(tc.get_value(0, 0, 0), std::logic_error);
    ASSERT_THROW(tc.get_value(1, 0, 0), std::logic_error);
    ASSERT_THROW(tc.get_value(0, 1, 1), std::logic_error);
    ASSERT_THROW(tc.get_value(0, 0, 4), std::logic_error);
}
TEST(test_byte_matrix, emptySetArgs_shouldThrow) {
    ByteMatrix tc(1);
    
    ASSERT_THROW(tc.set_value(0, 0, ByteVector{}), std::logic_error);
}
TEST(test_byte_matrix, byteRowSize_shouldBeZeroByDefault) {
    for(int tc_size : { 1, 20, 4000 }) {
        ByteMatrix tc(tc_size);
        ASSERT_NO_THROW(tc.create_empty_row());
        
        ASSERT_TRUE(std::ranges::equal(tc.get_value(0, 0, 1), ByteVector{ 0x00 }));
    }
}
TEST(test_byte_matrix, byteRowSize_shouldSetAndGetSuccess) {
    ByteMatrix tc(1);

    ASSERT_NO_THROW(tc.create_empty_row());
    ASSERT_NO_THROW(tc.set_value(0, 0, { 0xFF }));

    ASSERT_TRUE(std::ranges::equal(tc.get_value(0, 0, 1), ByteVector{ 0xFF }));
}
TEST(test_byte_matrix, byteRowSize_overflowing_shouldThrow) {
    ByteMatrix tc(1);
    ASSERT_NO_THROW(tc.create_empty_row());

    ASSERT_THROW(tc.set_value(1, 0, {0xFF}), std::logic_error);
    ASSERT_THROW(tc.set_value(0, 1, {0xDD}), std::logic_error);
    ASSERT_THROW(tc.set_value(3, 0, {0xDD}), std::logic_error);
    ASSERT_THROW(tc.get_value(1, 0, 1), std::logic_error);
    ASSERT_THROW(tc.get_value(3, 0, 1), std::logic_error);
    ASSERT_THROW(tc.get_value(0, 0, 2), std::logic_error);
    ASSERT_THROW(tc.get_value(0, 1, 3), std::logic_error);
}

std::array<ByteVector, 4> test_values = {
    ByteVector{ 0x00, 0x00, 0x07, 0xC0 }, // 1984
    ByteVector{ 0x01, 0x22, 0x40, 0xB7 }, // 19022007
    ByteVector{ 0x40, 0x49, 0x0f, 0xdb }, // 3.1415927
    ByteVector{ 0x40, 0x2d, 0xf8, 0x4d }  // 2.71828
};

TEST(test_byte_matrix, testValues_are_same_size_as_int_and_float) {
    EXPECT_EQ(test_values[0].size(), sizeof(int));
    EXPECT_EQ(test_values[1].size(), sizeof(int));
    EXPECT_EQ(test_values[2].size(), sizeof(float));
    EXPECT_EQ(test_values[3].size(), sizeof(float));
}

TEST(test_byte_matrix, intnfloatRowSize_SuccessSetGet) {
    ByteMatrix tc(sizeof(int) + sizeof(float));
    tc.create_empty_row();
    tc.create_empty_row();

    tc.set_value(0, 0, test_values[0]); 
    tc.set_value(1, 0, test_values[1]); 
    tc.set_value(0, sizeof(int), test_values[2]); 
    tc.set_value(1, sizeof(int), test_values[3]); 
    
    EXPECT_RANGEQ(tc.get_value(0, 0, sizeof(int)), test_values[0]);
    EXPECT_RANGEQ(tc.get_value(1, 0, sizeof(int)), test_values[1]);
    EXPECT_RANGEQ(tc.get_value(0, sizeof(int), sizeof(float)), test_values[2]);
    EXPECT_RANGEQ(tc.get_value(1, sizeof(int), sizeof(float)), test_values[3]);
}

TEST(test_byte_matrix, intnfloatRowSize_twistedOffset_Success) {
    ByteMatrix tc(sizeof(int) + sizeof(float));
    tc.create_empty_row();
    tc.create_empty_row();

    tc.set_value(0, 0, test_values[0]); 
    tc.set_value(0, 1, test_values[1]); 
    tc.set_value(0, 2, test_values[2]); 
    tc.set_value(0, 3, test_values[3]); 

    auto res = ByteVector { 
        test_values[0][0], 
        test_values[1][0], 
        test_values[2][0], 
        test_values[3][0], 
        test_values[3][1], 
        test_values[3][2], 
        test_values[3][3],
        0x00
    };
    EXPECT_RANGEQ(tc.get_value(0, 0, sizeof(int) + sizeof(float)), res);
}

TEST(test_byte_matrix, intnfloatRowSize_offsetOverflow_shouldThrow) {
    ByteMatrix tc(sizeof(int));
    tc.create_empty_row();
    tc.create_empty_row();

    EXPECT_THROW(tc.set_value(0, 1, test_values[0]), std::logic_error);
    EXPECT_THROW(tc.get_value(0, 1, sizeof(int)), std::logic_error);
}

TEST(test_byte_matrix, intnfloatRowSize_offsetOverflow_strictGuarantee) {
    ByteMatrix tc(sizeof(int));
    tc.create_empty_row();
    tc.create_empty_row();

    EXPECT_THROW(tc.set_value(0, 1, test_values[0]), std::logic_error);
    EXPECT_THROW(tc.get_value(0, 1, 4), std::logic_error);

    EXPECT_RANGEQ(tc.get_value(0, 0, sizeof(int)), ByteVector(sizeof(int), 0x00));
}

TEST(test_byte_matrix, constQualifier_shouldCompile) {
    ByteMatrix tc(sizeof(int));
    tc.create_empty_row();
    tc.create_empty_row();

    const ByteMatrix& bm = tc;

    std::ignore = bm.get_value(0, 0, 1);
}

}
