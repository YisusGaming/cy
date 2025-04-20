cmake_minimum_required(VERSION 3.31)

project(CY VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_executable(types "${CMAKE_CURRENT_SOURCE_DIR}/tests/types.cpp")
add_executable(maybe "${CMAKE_CURRENT_SOURCE_DIR}/tests/maybe.cpp")
add_executable(result "${CMAKE_CURRENT_SOURCE_DIR}/tests/result.cpp")