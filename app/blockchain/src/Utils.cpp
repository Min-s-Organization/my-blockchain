#include "Utils.h"
#include <sstream>
#include <iomanip>

namespace block_chain
{
  std::string Utils::bytesToHex(const std::vector<uint8_t> &bytes)
  {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (const auto &byte : bytes)
    {
      ss << std::setw(2) << static_cast<int>(byte);
    }

    return ss.str();
  }

  std::vector<uint8_t> Utils::stringToBytes(const std::string &data)
  {
    std::vector<uint8_t> bytes;
    bytes.reserve(data.size());
    for (const auto &byte : data)
      bytes.push_back(static_cast<uint8_t>(byte));
    return bytes;
  }
}
