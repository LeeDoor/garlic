add_library(pch_gtest INTERFACE)
target_precompile_headers(pch_gtest INTERFACE
    gtest.hpp
)
target_link_libraries(pch_gtest INTERFACE GTest::gtest_main)
