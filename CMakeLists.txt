cmake_minimum_required(VERSION 4.1)
project(garlic)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 23)
set(SANITIZERS_FLAGS "-fsanitize=address,undefined -Wall -Wextra -Wpedantic -Werror -fmax-errors=1")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${SANITIZERS_FLAGS})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${SANITIZERS_FLAGS})

enable_testing()
add_subdirectory(src)
