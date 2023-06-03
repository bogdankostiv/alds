cmake_minimum_required(VERSION 3.15)
project(alds-example C)

set(CMAKE_C_STANDARD 11)

set(SRC
        main.c
        simple.c
        )

add_executable(alds-example ${SRC})

target_link_libraries(alds-example
                      PRIVATE alds)