# ALDS - ALgorithms and Data Structures

This is a project that contains a set of different algorithms and data structures. It is written in pure C by using standartd C libraries only. The project is also oriented on systems with limited resources, so it allows to use custom and static memory allocators.

I decided to crate this project to get some useful data structures, algorithms and tools in a single place. Another reason is to learn more deeply different complicated structures.

## Components

The project consists of 3 parts:
* The library itself in the `lib` folder. It can be compiled as a static library or used by importing sourse code;
* Unit tests;
* Example application. Just a simple example on how to use the primitives;

## Build

Clean build dir: `rm -rf build`

Library: 
* build `cmake -S . -B ./build && cmake --build ./build --target alds`

Tests:
* build: `cmake -S . -B ./build && cmake --build ./build --target alds-tests`
* run: `./build/tests/alds-tests`

Demo
* build: `cmake -S . -B ./build && cmake --build ./build --target alds-example`
* run: `./build/example/alds-example`

## License
MIT. Feel free to use the library. Modification is not prohibited.  
