#pragma once

#include <vector>
#include <cstdint>
#include <ctime>
#include <openssl/ssl.h>
#include "Hash.h"

namespace block_chain
{
  class Block
  {
  public:
    Block(const std::vector<uint8_t> &data, const std::vector<uint8_t> &prev_hash);
    ~Block() = default;
    const Hash &getHash() const { return hash_; };

  private:
    time_t timestamp_;
    std::vector<uint8_t> data_;
    Hash hash_;
    std::vector<uint8_t> prev_hash_;
  };
}