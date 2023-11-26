cmake_minimum_required(VERSION 3.15)
project(alds C)

set(CMAKE_C_STANDARD 11)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DVERBOSE)
    add_definitions(-DDEBUG)
endif ()

include_directories(include)

set(LIB_PRIVATE_HEADER_FILES

        )

set(LIB_SRC
        src/alds.c
        src/alds_log.c
        src/data/alds_alloc.c
        src/basic/stack.c
        src/basic/queue.c
        src/basic/linked_list.c
        )

SET(LIB_TYPE STATIC)

add_library(alds ${LIB_SRC})

target_include_directories(alds
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
