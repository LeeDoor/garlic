set(DEPENDENCIES )

add_library(table STATIC
    byte_matrix.cpp
)
target_link_libraries(table PUBLIC ${GENERAL_LINK_LIBRARIES})
target_include_directories(table PUBLIC ../abstract_syntax_tree ${GENERAL_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR})
