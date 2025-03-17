aux_source_directory(. SRC_LIST)
aux_source_directory(./protobuf PROTO_SRC) 

include_directories(${CMAKE_BINARY_DIR}/_deps/muduo-src)
include_directories(${PROJECT_SOURCE_DIR}/src/protobuf)
include_directories(${PROJECT_SOURCE_DIR}/src/include)  

add_library(rpc SHARED ${SRC_LIST} ${PROTO_SRC})
target_link_libraries(rpc muduo_net muduo_base pthread protobuf) 
