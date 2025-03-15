find_package(OpenSSL REQUIRED)

aux_source_directory(. SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/protobuf PROTOBUF_SRC_LIST)

find_package(leveldb REQUIRED)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/protobuf)

add_library(Block SHARED ${SRC_LIST} ${PROTOBUF_SRC_LIST})
add_library(Blockchain SHARED ${SRC_LIST})

target_link_libraries(Block PUBLIC OpenSSL::Crypto ${Protobuf_LIBRARIES} leveldb pthread)
target_link_libraries(Blockchain PUBLIC Block leveldb pthread)