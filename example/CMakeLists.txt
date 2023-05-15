cmake_minimum_required(VERSION 3.15)
project(alds-example C)

set(CMAKE_C_STANDARD 11)

add_executable(alds-example main.c)

target_link_libraries(alds-example
                      PRIVATE alds)