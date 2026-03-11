cmake_minimum_required(VERSION 3.28)
project(mercurial VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(merc ${SOURCES})

target_include_directories(merc PRIVATE src)

# Warnings
target_compile_options(merc PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

# Debug/Release
target_compile_options(merc PRIVATE
    $<$<CONFIG:Debug>:-g -O0 -DDEBUG>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)