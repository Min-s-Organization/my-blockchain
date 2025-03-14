#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace block_chain
{
  class Utils
  {
  public:
    static std::string bytesToHex(const std::vector<uint8_t> &bytes);
    static std::vector<uint8_t> stringToBytes(const std::string &data);
    static std::string intToHex(int64_t value);
  };
}