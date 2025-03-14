#include "Block.h"
#include <iostream>
#include "Utils.h"

namespace block_chain
{
  Block::Block(const std::vector<uint8_t> &data, const std::vector<uint8_t> &prev_hash)
      : data_(data), prev_hash_(prev_hash), hash_(Hash(std::vector<const uint8_t *>({data.data(), prev_hash.data()})))
  {
  }

  void Block::print() const
  {
    std::cout << "Block hash: " << hash_.toHex() << std::endl;
    std::cout << "Block prev hash: " << Utils::bytesToHex(prev_hash_) << std::endl;
    std::cout << "Block timestamp: " << timestamp_ << std::endl;
    std::cout << "Block data: " << Utils::bytesToHex(data_) << std::endl;
  }
}