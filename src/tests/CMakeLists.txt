find_package(GTest)
include(FetchContent)
if(NOT ${GTest_FOUND})
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
        FIND_PACKAGE_ARGS NAMES GTest
    )
    FetchContent_MakeAvailable(googletest)
endif()

include(gtest_pch.cmake)

add_subdirectory(unit)
add_subdirectory(sql_parser)
