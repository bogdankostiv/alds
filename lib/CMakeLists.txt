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
        include/alds_memory.h
        include/alds_error.h
        include/alds_log.h
        include/simple/stack.h
        )

set(LIB_PRIVATE_HEADER_FILES

        )

set(LIB_SRC
        src/alds.c
        src/alds_memory.c
        src/alds_log.c
        src/simple/stack.c
        src/simple/queue.c
        )

SET(LIB_TYPE STATIC)

add_library(alds ${LIB_INSTALL_HEADER_FILES} ${LIB_PRIVATE_HEADER_FILES} ${LIB_SRC})

target_include_directories(alds
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
