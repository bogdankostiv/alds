cmake_minimum_required(VERSION 3.15)
project(alds C)

set(CMAKE_C_STANDARD 11)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DVERBOSE)
    add_definitions(-DDEBUG)
endif ()


include_directories(include)

set(LIB_INSTALL_HEADER_FILES
        include/alds.h
        include/alds_memgmnt.h
        include/alds_error.h
        )

set(LIB_PRIVATE_HEADER_FILES

        )

set(LIB_SRC
        src/alds.c
        src/alds_memgmnt.c
        )

SET(LIB_TYPE STATIC)

add_library(alds ${LIB_INSTALL_HEADER_FILES} ${LIB_PRIVATE_HEADER_FILES} ${LIB_SRC})
