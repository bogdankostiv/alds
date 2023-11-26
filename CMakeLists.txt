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

add_custom_target(clang-format
    COMMAND echo "Run clang-format"
    COMMAND find ../example -name "*.c" -o -name "*.h" -o -name "*.cpp" | sed 's| |\\ |g' | xargs clang-format -i -style=file:../.clang-format
    COMMAND find ../lib -name "*.c" -o -name "*.h" -o -name "*.cpp" | sed 's| |\\ |g' | xargs clang-format -i -style=file:../.clang-format
    COMMAND find ../tests -name "*.c" -o -name "*.h" -o -name "*.cpp" | sed 's| |\\ |g' | xargs clang-format -i -style=file:../.clang-format
)