include_directories(${PROJECT_SOURCE_DIR}/example)
include_directories(${CMAKE_BINARY_DIR}/_deps/muduo-src)

add_subdirectory(receiver)
add_subdirectory(sender)