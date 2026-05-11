cmake_minimum_required(VERSION 3.22)

project(media_list LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(media_list
    src/main.cpp
    src/CatalogParser.cpp
)

target_include_directories(media_list PRIVATE include)

