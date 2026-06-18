cmake_minimum_required(VERSION 3.4.1)
project(plugin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(plugin SHARED plugin.cpp)

# Путь к vendor
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

# Подключаем Dobby (путь без папки lib)
add_library(dobby STATIC IMPORTED)
set_target_properties(dobby PROPERTIES IMPORTED_LOCATION ${VENDOR_DIR}/Dobby/arm64-v8a/libdobby.a)

# Добавляем заголовки
target_include_directories(plugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${VENDOR_DIR}/Dobby/include
    ${VENDOR_DIR}/nlohmann
)

# Линкуем
target_link_libraries(plugin
    dobby
    log
    dl
)
