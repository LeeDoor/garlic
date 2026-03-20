add_library(condition_tree STATIC
    cell_int_value.cpp
    cell_float_value.cpp
)
target_link_libraries(condition_tree PUBLIC ${GENERAL_LINK_LIBRARIES})
target_include_directories(condition_tree PUBLIC ../table ${GENERAL_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR})
