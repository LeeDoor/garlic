include(gtest_pch.cmake)

set(GENERAL_LINK_LIBRARIES ${GENERAL_LINK_LIBRARIES} pch_gtest)

set(DEPENDENCIES 
    table
)

add_executable(tests 
    test_byte_matrix.cpp
)
target_link_libraries(tests PUBLIC
    ${GENERAL_LINK_LIBRARIES}
    ${DEPENDENCIES}
)
target_include_directories(tests PUBLIC 
    ${GENERAL_INCLUDE_DIRECTORIES}
    ${DEPENDENCIES}
)

add_test(NAME test COMMAND tests)
