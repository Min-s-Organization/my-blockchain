#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace block_chain
{
  class Hash
  {
  public:
    Hash(const std::vector<const uint8_t *> &input);
    ~Hash() = default;
    const std::vector<uint8_t> &toBytes() const { return hash_; };
    std::string toHex() const;

  private:
    std::vector<uint8_t> hash_;
  };
}