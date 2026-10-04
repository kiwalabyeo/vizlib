### vizlib

vizlib description to be completed at the end of the project when we all know what exactly it does.

## Objectives
Build a matrix library with a clean, easy-to-read API.
Practise separating interface from implementation in C++.
Handle bad input safely instead of crashing.
Test every operation, including edge cases.

## Main features
Create matrices from nested lists, or as zero or identity matrices
Add, subtract and multiply matrices, and scale by a number
Transpose, determinant and inverse
Solve Ax = b using Gaussian elimination
Print matrices neatly to the console
Throw clear errors when sizes don't match

## Project structure
include/matrixlite/: public headers, the only files users include
src/: implementation files
tests/: unit tests using doctest
examples/: small programs showing how to use the library
reports/: weekly progress reports
docs/: detailed documentation for each module

## Requirements
A C++17 compiler (tested on GCC 12, Clang 15 and MSVC 2022)
CMake 3.16 or newer

doctest is included in third_party/, so there's nothing else to install.

## Building
git clone https://github.com/example/matrixlite.git
cd matrixlite
cmake -S . -B build
cmake --build build

On Windows, run the same commands in the Developer Command Prompt.

## Using the library
<cpp
#include matrixlite/matrix.hpp
matrixlite::Matrix a = {{1, 2}, {3, 4}};
matrixlite::Matrix b = matrixlite::Matrix::identity(2);
auto c = a * b;
std::cout << c;>

See docs/matrix.md for the full list of functions.

## Examples
examples/basic_ops.cpp: addition, multiplication and printing
examples/solve_system.cpp: solves a 3x3 system of equations

Run them with ./build/examples/basic_ops.

## Running the tests
cd build
ctest --output-on-failure

All tests run automatically on every pull request through GitHub Actions.

## Limitations
Only dense matrices; sparse matrices aren't supported.
Only double values, not complex numbers.
Inverse and determinant get slow above about 500x500.

## Contributions
Jane Doe (@janedoe): wrote the Matrix class, addition and multiplication, and the tests for them.
John Smith (@jsmith): wrote Gaussian elimination, determinant and inverse, and set up CMake and CI.

## How we collaborated

We split the work by module and agreed the header files first, so each of us could work without waiting on the other. Every change went through a pull request that needed three people's approval to merge to main. We met every Sunday to plan the week and wrote the weekly report together.

## AI use

See the AI Use section in each weekly report in reports/.

## License

MIT. See LICENSE.