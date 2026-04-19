add_library(abstract_syntax_tree STATIC
    binary_logical_condition.cpp
    cell_float_value.cpp
    cell_int_value.cpp
    cell_string_view_value.cpp
    cell_boolean_value.cpp
    compare_condition.cpp
    condition_select_query.cpp
    expression_select_query.cpp
    string_query_result.cpp
    table_value_expression.cpp
    table_value_gatherer_impl.cpp
    unary_logical_condition.cpp
    binary_math_expression.cpp
    unary_math_expression.cpp
)
target_link_libraries(abstract_syntax_tree PUBLIC ${GENERAL_LINK_LIBRARIES})
target_include_directories(abstract_syntax_tree PUBLIC ../table ${GENERAL_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR})
