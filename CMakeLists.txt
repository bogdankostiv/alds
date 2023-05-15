cmake_minimum_required(VERSION 3.15)
project(alds_global)

add_custom_target(ut ALL
  COMMENT "Unit test target"
  COMMAND cmake -S ./tests -B ./tests/build && cmake --build ./tests/build
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/
)