set(DEPENDENCIES )

add_library(table STATIC
    typed_table.cpp
    byte_matrix.cpp
)
target_link_libraries(table PUBLIC ${GENERAL_LINK_LIBRARIES})
target_include_directories(table PUBLIC ${GENERAL_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR})
