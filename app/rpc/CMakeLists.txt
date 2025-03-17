cmake_minimum_required(VERSION 3.20)
project(rpc)

# 添加 FetchContent
include(FetchContent)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# 获取 muduo
FetchContent_Declare(
    muduo
    GIT_REPOSITORY https://github.com/chenshuo/muduo.git
    GIT_TAG master
)
FetchContent_MakeAvailable(muduo)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${CMAKE_BINARY_DIR}/_deps/muduo-src)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/protobuf)
include_directories(${PROJECT_SOURCE_DIR}/example)
include_directories(${MUDUO_INCLUDE_DIRS})
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)
add_subdirectory(example)