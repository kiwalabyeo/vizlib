cmake_minimum_required(VERSION 3.16)
project(vizlib VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(vizlib
    src/version.cpp
)

target_include_directories(vizlib
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/third_party
)

if(MSVC)
    target_compile_options(vizlib PRIVATE /W4)
else()
    target_compile_options(vizlib PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(VIZLIB_BUILD_TESTS "Build the tests" ON)
if(VIZLIB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()