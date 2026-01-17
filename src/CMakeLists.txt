include(pch.cmake)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} GTest::gtest_main SFML::Graphics pch_std)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC headers)

add_test(NAME testfirst COMMAND ${CMAKE_PROJECT_NAME})
