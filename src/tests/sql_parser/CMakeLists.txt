add_test(NAME test_sql_parser
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/input_output_comparator.py $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
)

