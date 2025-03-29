include_directories(../src/include)

find_package(OpenSSL REQUIRED)

add_executable(BlockTest BlockTest.cpp ../src/include/Block.h)
add_executable(BlockchainTest BlockchainTest.cpp ../src/include/Blockchain.h)
add_executable(ProtobufTest ProtobufTest.cpp ../src/include/Block.h ../src/protobuf/block.pb.cc)
add_executable(LevelDBTest LevelDBTest.cpp)
add_executable(WalletTest WalletTest.cpp ../src/include/Wallet.h ../src/Wallet.cpp ../src/include/Hash.h ../src/Hash.cpp ../src/include/Utils.h ../src/Utils.cpp)

target_link_libraries(BlockTest PRIVATE Block)
target_link_libraries(BlockchainTest PRIVATE Blockchain)
target_link_libraries(ProtobufTest PRIVATE Block)
target_link_libraries(LevelDBTest PRIVATE leveldb)
target_link_libraries(WalletTest PRIVATE OpenSSL::Crypto)