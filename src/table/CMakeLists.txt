set(DEPENDENCIES )

add_library(table STATIC
    table.cpp
    table_content.cpp
)
target_link_libraries(table PUBLIC ${GENERAL_LINK_LIBRARIES})
target_include_directories(table PUBLIC ${GENERAL_INCLUDE_DIRECTORIES})
