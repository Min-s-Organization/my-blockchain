#include "Block.h"
#include <iostream>

using namespace block_chain;

int main()
{
  std::cout << "\nBlockTest running..." << std::endl;

  std::vector<uint8_t> data = {0x01, 0x02, 0x03};
  Hash prev_hash = Hash();
  Block block(std::vector<Transaction>(), prev_hash);
  block.print();

  return 0;
}