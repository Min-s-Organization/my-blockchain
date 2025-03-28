#include "Block.h"
#include <iostream>

using namespace block_chain;

int main()
{
  std::cout << "\nProtobufTest running..." << std::endl;

  std::cout << "Original block:" << std::endl;
  std::vector<uint8_t> data = {0x01, 0x02, 0x03};
  Hash prev_hash = Hash();
  Block block(std::vector<Transaction>(), prev_hash);
  block.print();

  std::vector<uint8_t> serialized_data;
  serialized_data = Block::serialize(block);

  std::cout << "\nDeserialized block:" << std::endl;
  Block deserialized_block = Block::deserialize(serialized_data);
  deserialized_block.print();

  return 0;
}