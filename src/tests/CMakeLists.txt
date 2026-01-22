include(gtest_pch.cmake)

set(GENERAL_LINK_LIBRARIES ${GENERAL_LINK_LIBRARIES} pch_gtest)

set(DEPENDENCIES 
    table
)

add_executable(tests test_table_content.cpp)
target_include_directories(tests PUBLIC 
    ${GENERAL_INCLUDE_DIRECTORIES}
    ${DEPENDENCIES}
)
target_link_libraries(tests PUBLIC
    ${GENERAL_LINK_LIBRARIES}
    ${DEPENDENCIES}
)

add_test(NAME test COMMAND tests)
