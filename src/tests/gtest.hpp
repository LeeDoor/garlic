#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define EXPECT_RANGEQ(a, b) EXPECT_TRUE(std::ranges::equal(a, b));

using ::testing::Return;

