add_test(NAME test_sql_parser
    COMMAND python3
        ${CMAKE_CURRENT_SOURCE_DIR}/input_output_comparator.py
        ${CMAKE_CURRENT_SOURCE_DIR}/select_query/select_only
        $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
)

add_test(NAME test_sql_parser_from_clause
    COMMAND python3
        ${CMAKE_CURRENT_SOURCE_DIR}/input_output_comparator.py
        ${CMAKE_CURRENT_SOURCE_DIR}/select_query/from_clause
        $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
)

set_tests_properties(
    test_sql_parser
    test_sql_parser_from_clause
    PROPERTIES DEPENDS "unit tests"
)
