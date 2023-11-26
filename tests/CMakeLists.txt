cmake_minimum_required(VERSION 3.15)
project(alds-tests C)

set(CMAKE_C_STANDARD 11)

option(BUILD_CODE_COVERAGE "Build test with the code coverage artifacts" OFF)

add_compile_options(-Wall)
if(APPLE)
    add_compile_options(-Wno-error=unused-command-line-argument)
endif(APPLE)

# Add definition for tests-related code
add_definitions(-DUNIT_TEST)

# Verbose mode enabled
add_definitions(-DVERBOSE)
# set the lower log level to test it with UT
add_definitions(-DALDS_LOG_LEVEL=0)

# Add Cmoka for UT
include(FetchContent)

FetchContent_Declare(
        cmocka
        GIT_REPOSITORY https://gitlab.com/cmocka/cmocka.git
        GIT_TAG        cmocka-1.1.5
        GIT_SHALLOW    1
)

set(WITH_STATIC_LIB ON CACHE BOOL "CMocka: Build with a static library" FORCE)
set(WITH_CMOCKERY_SUPPORT OFF CACHE BOOL "CMocka: Install a cmockery header" FORCE)
set(WITH_EXAMPLES OFF CACHE BOOL "CMocka: Build examples" FORCE)
set(UNIT_TESTING OFF CACHE BOOL "CMocka: Build with unit testing" FORCE)
set(PICKY_DEVELOPER OFF CACHE BOOL "CMocka: Build with picky developer flags" FORCE)

FetchContent_MakeAvailable(cmocka)


include_directories(../lib/include)

set(SRC_TO_TEST
        ../lib/src/alds.c
        ../lib/src/alds_log.c
        ../lib/src/data/alds_alloc.c
        ../lib/src/basic/stack.c
        ../lib/src/basic/queue.c
        )

set(SRC
        test.c
        log_tests.c
        data_tests.c
        basic/stack_tests.c
        basic/queue_tests.c
        )

add_executable(alds-tests
        ${SRC}
        ${SRC_TO_TEST}
        )


target_link_libraries(alds-tests cmocka-static)

# Add code coverage support
if(BUILD_CODE_COVERAGE)
    include(CodeCoverage.cmake)
    append_coverage_compiler_flags()

    setup_target_for_coverage_lcov(
            NAME coverage                                                              # New target name
            EXECUTABLE tml-test -j ${PROCESSOR_COUNT}                                  # Executable in PROJECT_BINARY_DIR
            DEPENDENCIES tml-test                                                      # Dependencies to build first
            EXCLUDE "${CMAKE_SOURCE_DIR}/src/*" "${CMAKE_SOURCE_DIR}/../tool/src/*"    # Patterns to exclude (can be relative
            ./ #  to BASE_DIRECTORY, with CMake 3.4+)
            NO_DEMANGLE                                                                # Don't demangle C++ symbols
            #  even if c++filt is found
    )
endif(BUILD_CODE_COVERAGE)
