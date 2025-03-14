#include "Block.h"

namespace block_chain
{
  Block::Block(const std::vector<uint8_t> &data, const std::vector<uint8_t> &prev_hash)
      : data_(data), prev_hash_(prev_hash), hash_(Hash(std::vector<const uint8_t *>({data.data(), prev_hash.data()})))
  {
  }
}