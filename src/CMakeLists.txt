include(pch.cmake)

set(GENERAL_LINK_LIBRARIES pch_std)
set(GENERAL_INCLUDE_DIRECTORIES 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

set(DEPENDENCIES 
    table
    abstract_syntax_tree
    sql_parser
)

foreach(DEP ${DEPENDENCIES})
    add_subdirectory(${DEP})
endforeach()

add_subdirectory(tests)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    ${DEPENDENCIES}
)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
    ${DEPENDENCIES}
    ${GENERAL_INCLUDE_DIRECTORIES}
)

