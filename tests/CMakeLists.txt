cmake_minimum_required(VERSION 3.15)
project(alds-test C)

set(CMAKE_C_STANDARD 11)

option(BUILD_CODE_COVERAGE "Build test with the code coverage artifacts" OFF)

add_compile_options(-Wall)

# Add definition for tests-related code
add_definitions(-DUNIT_TEST)

# Verbose mode enabled
add_definitions(-DVERBOSE)

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

#include_directories(../tool/src)
#include_directories(../)

include_directories(../lib/include)
#include_directories(../lib/include/tools)

#include_directories(../lib/src/asn1)
#include_directories(../lib/src/asn1_decoding_fsm)
#include_directories(../lib/src/public_key)
#include_directories(../lib/src/x509)
#include_directories(../lib/src/abstractions)
#include_directories(../lib/src)

#include_directories(src)
#include_directories(src/x509)
#include_directories(src/asn1)
#include_directories(src/utilities)
#include_directories(src/other)

set(HEADER_FILES_TO_TEST
        ../lib/include/alds.h
        )

set(SRC_TO_TEST
        ../lib/src/alds.c
        ../lib/src/alds_memgmnt.c
        )

set(SRC
        test.c
        )

set(HDR
        ${HEADER_FILES_TO_TEST}
        )

add_executable(alds-test
        ${SRC}
        ${SRC_TO_TEST}
        )


target_link_libraries(alds-test cmocka-static)

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
