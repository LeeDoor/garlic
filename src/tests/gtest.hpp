#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define EXPECT_RANGEQ(a, b) EXPECT_TRUE(std::ranges::equal(a, b));

using ::testing::Return;
