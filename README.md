# Educational project to learn algorithms and data structures

## Build

Clean build dir: `rm -rf ./lib/build`

Library: 
* build `cmake -S . -B ./build && cmake --build ./build --target alds`

Tests:
* build: `cmake -S . -B ./build && cmake --build ./build --target alds-tests`
* run: `./build/tests/alds-tests`

Demo
* build: `cmake -S . -B ./build && cmake --build ./build --target alds-example`
* run: `./build/example/alds-example`

