include_directories(../src/include)

add_executable(BlockTest BlockTest.cpp ../src/include/Block.h)
add_executable(BlockchainTest BlockchainTest.cpp ../src/include/Blockchain.h)

target_link_libraries(BlockTest PRIVATE Block)
target_link_libraries(BlockchainTest PRIVATE Blockchain)