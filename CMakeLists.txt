cmake_minimum_required(VERSION 3.4.1)
project(plugin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(plugin SHARED plugin.cpp)

target_include_directories(plugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_library(log-lib log)
target_link_libraries(plugin ${log-lib} dl)
