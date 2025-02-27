include_directories(../src/include)

add_executable(BlockTest BlockTest.cpp ../src/include/Block.h)

target_link_libraries(BlockTest PRIVATE Block)