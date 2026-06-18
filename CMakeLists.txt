cmake_minimum_required(VERSION 3.4.1)
project(plugin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Скачиваем и собираем Dobby
include(FetchContent)
FetchContent_Declare(
    dobby
    GIT_REPOSITORY https://github.com/jmpews/Dobby.git
    GIT_TAG master
)
FetchContent_MakeAvailable(dobby)

add_library(plugin SHARED plugin.cpp)

target_include_directories(plugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${dobby_SOURCE_DIR}/include
)

target_link_libraries(plugin
    dobby
    log
    dl
)
