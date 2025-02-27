#include "Block.h"
#include "Utils.h"
#include <iostream>

using namespace block_chain;

int main()
{
  std::cout << "Test running..." << std::endl;

  Block block;
  block.setHash();
  const auto &hash_bytes = block.getHash();

  std::cout << "Create a block: " << Utils::bytesToHexString(hash_bytes) << std::endl;

  return 0;
}