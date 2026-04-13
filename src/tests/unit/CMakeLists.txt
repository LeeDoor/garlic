set(TEST_EXECUTABLE_NAME unit_test)
set(GENERAL_LINK_LIBRARIES ${GENERAL_LINK_LIBRARIES} pch_gtest)
set(DEPENDENCIES 
    table
    abstract_syntax_tree
    sql_parser
)

add_executable(${TEST_EXECUTABLE_NAME} 
    test_byte_matrix.cpp
    test_typed_table.cpp
    test_cell_int_value.cpp
    test_cell_float_value.cpp
    test_cell_string_view_value.cpp
    test_cell_string_value.cpp
    test_constant_expression.cpp
    test_table_value_gatherer.cpp
    test_table_value_expression.cpp
    test_math_expression.cpp
    test_binary_unary_math_expression.cpp
    test_select_queries.cpp
    test_compare_condition.cpp
    test_binary_logical_condition.cpp
    test_unary_logical_condition.cpp
    test_sql_lexer_make_string.cpp
)
target_link_libraries(${TEST_EXECUTABLE_NAME} PUBLIC
    ${GENERAL_LINK_LIBRARIES}
    ${DEPENDENCIES}
)
target_include_directories(${TEST_EXECUTABLE_NAME} PUBLIC 
    ${GENERAL_INCLUDE_DIRECTORIES}
    ${DEPENDENCIES}
    ${CMAKE_SOURCE_DIR}/src/abstract_syntax_tree
    mocks
)

add_test(NAME run_all_tests COMMAND ${TEST_EXECUTABLE_NAME})
