include(pch.cmake)

set(GENERAL_LINK_LIBRARIES pch_std)
set(GENERAL_INCLUDE_DIRECTORIES 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

set(DEPENDENCIES 
    table
)

foreach(DEP ${DEPENDENCIES})
    add_subdirectory(${DEP})
endforeach()

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    ${DEPENDENCIES}
    ${GENERAL_LINK_LIBRARIES}
)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
    ${DEPENDENCIES}
    ${GENERAL_INCLUDE_DIRECTORIES}
)

