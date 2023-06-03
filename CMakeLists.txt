cmake_minimum_required(VERSION 3.15)
project(alds-root C)

set(CMAKE_C_STANDARD 11)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DVERBOSE)
    add_definitions(-DDEBUG)
endif ()

add_subdirectory(lib)
add_subdirectory(example)
add_subdirectory(tests)
