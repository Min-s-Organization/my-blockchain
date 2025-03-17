cmake_minimum_required(VERSION 3.10)
project(Reactor)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/include)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(test)