cmake_minimum_required(VERSION 4.1)
project(batadase)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)
set(SANITIZERS_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${SANITIZERS_FLAGS})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${SANITIZERS_FLAGS})

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
    FIND_PACKAGE_ARGS NAMES GTest
)
FetchContent_MakeAvailable(googletest)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
add_subdirectory(src)
