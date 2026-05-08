set(DEPENDENCIES )

add_library(terminal_api STATIC
    manual_io.cpp
)
target_link_libraries(terminal_api PUBLIC ${GENERAL_LINK_LIBRARIES})
target_include_directories(terminal_api PUBLIC ${GENERAL_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR})
