include(gtest_pch.cmake)


set(TEST_EXECUTABLE_NAME test_exe)
set(GENERAL_LINK_LIBRARIES ${GENERAL_LINK_LIBRARIES} pch_gtest)
set(DEPENDENCIES 
    table
)

add_executable(${TEST_EXECUTABLE_NAME} 
    test_byte_matrix.cpp
    test_typed_table.cpp
)
target_link_libraries(${TEST_EXECUTABLE_NAME} PUBLIC
    ${GENERAL_LINK_LIBRARIES}
    ${DEPENDENCIES}
)
target_include_directories(${TEST_EXECUTABLE_NAME} PUBLIC 
    ${GENERAL_INCLUDE_DIRECTORIES}
    ${DEPENDENCIES}
)

add_test(NAME run_all_tests COMMAND ${TEST_EXECUTABLE_NAME})
