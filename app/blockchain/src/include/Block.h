#pragma once

#include <vector>
#include <cstdint>
#include <ctime>
#include <openssl/ssl.h>

namespace block_chain
{
  class Block
  {
  public:
    Block() { timestamp_ = std::time(NULL); };
    ~Block() = default;
    void setHash();
    const std::vector<uint8_t> &getHash() const { return hash_; };

  private:
    time_t timestamp_;
    std::vector<uint8_t> data_;
    std::vector<uint8_t> hash_;
    std::vector<uint8_t> prev_hash_;
  };
}