#pragma once

#include <gtest/gtest.h>
#define EXPECT_RANGEQ(a, b) EXPECT_TRUE(std::ranges::equal(a, b));

