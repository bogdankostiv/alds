cmake_minimum_required(VERSION 3.15)
project(alds-example C)

set(CMAKE_C_STANDARD 11)

# Allow logging
add_definitions(-DVERBOSE)
add_definitions(-DALDS_LOG_LEVEL=0)

set(SRC
        main.c
        simple.c
        )

add_executable(alds-example ${SRC})

target_link_libraries(alds-example
                      PRIVATE alds)