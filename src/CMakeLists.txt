add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} GTest::gtest_main)

add_test(NAME testfirst COMMAND ${CMAKE_PROJECT_NAME})
