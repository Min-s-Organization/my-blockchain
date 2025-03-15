include_directories(../src/include)

add_executable(BlockTest BlockTest.cpp ../src/include/Block.h)
add_executable(BlockchainTest BlockchainTest.cpp ../src/include/Blockchain.h)
add_executable(ProtobufTest ProtobufTest.cpp ../src/include/Block.h ../src/protobuf/block.pb.cc)
add_executable(LevelDBTest LevelDBTest.cpp)

target_link_libraries(BlockTest PRIVATE Block)
target_link_libraries(BlockchainTest PRIVATE Blockchain)
target_link_libraries(ProtobufTest PRIVATE Block)
target_link_libraries(LevelDBTest PRIVATE leveldb)