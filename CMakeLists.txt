cmake_minimum_required(VERSION 3.4.1)
project(plugin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(plugin SHARED plugin.cpp)

# Подключаем Dobby
add_library(dobby STATIC IMPORTED)
set_target_properties(dobby PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/Dobby/lib/arm64-v8a/libdobby.a)

target_include_directories(plugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Dobby/include
)

target_link_libraries(plugin
    dobby
    log
    dl
)
