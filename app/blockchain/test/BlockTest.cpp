#include "Block.h"
#include <iostream>

using namespace block_chain;

int main()
{
  std::cout << "Test running..." << std::endl;

  std::vector<uint8_t> data = {0x01, 0x02, 0x03};
  std::vector<uint8_t> prev_hash = {0x04, 0x05, 0x06};
  Block block(data, prev_hash);
  std::cout << "Block hash: " << block.getHash().toHex() << std::endl;

  return 0;
}