#include "Block.h"
#include <iostream>
#include "Utils.h"
#include <vector>

namespace block_chain
{
  Block::Block(const std::vector<uint8_t> &data, const Hash &prev_hash)
      : data_(data), prev_hash_(prev_hash)
  {
    timestamp_ = time(nullptr);
    std::vector<const uint8_t *> input;
    input.push_back(data.data());

    if (prev_hash.toBytes().size() > 0)
      input.push_back(prev_hash.toBytes().data());

    input.push_back(reinterpret_cast<const uint8_t *>(&timestamp_));
    hash_ = Hash(input);
  }

  void Block::print() const
  {
    std::cout << "Block hash: " << hash_.toHex() << std::endl;
    std::cout << "Block prev hash: " << prev_hash_.toHex() << std::endl;
    std::cout << "Block timestamp: " << timestamp_ << std::endl;
    std::cout << "Block data: " << Utils::bytesToHex(data_) << std::endl;
  }
}