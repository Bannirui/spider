cmake_minimum_required(VERSION 3.21)
project(spider)

set(CMAKE_CXX_STANDARD 11)

include_directories(
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/src/headers"
)

file(GLOB_RECURSE _headers src/headers/*.h)
file(GLOB_RECURSE _sources src/crawler/*.cpp)

add_executable(spider src/main.cpp ${_headers} ${_sources})