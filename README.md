# Educational project to learn algorithms and data structures

## Build

Library:
* clean `rm -rf ./lib/build`
* build `cmake -S ./lib -B ./lib/build && cmake --build ./lib/build`
* clean-build: `rm -rf ./lib/build && cmake -S ./lib -B ./lib/build && cmake --build ./lib/build`

Tests:
* clean `rm -rf ./tests/build`
* build `cmake -S ./tests -B ./tests/build && cmake --build ./tests/build`
* clean-build: `rm -rf ./tests/build && cmake -S ./tests -B ./tests/build && cmake --build ./tests/build`