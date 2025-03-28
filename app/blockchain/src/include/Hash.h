#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace block_chain
{
  class Hash
  {
  public:
    Hash(const std::vector<uint8_t> &input, bool is_hash = false);
    Hash(const std::string &input);
    Hash();
    ~Hash() = default;
    const std::vector<uint8_t> &toBytes() const { return hash_; };
    std::string toHex() const;
    bool isEmpty() const { return hash_.empty(); }

  private:
    std::vector<uint8_t> hash_;
  };
}