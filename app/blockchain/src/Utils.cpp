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

  std::vector<uint8_t> Utils::hexToBytes(const std::string &hex)
  {
    std::vector<uint8_t> bytes;
    // if (hex.size() % 2 != 0)
    // {
    //   exit(-1);
    // }
    for (size_t i = 0; i < hex.size(); i += 2)
    {
      std::string byte_str = hex.substr(i, 2);
      bytes.push_back(static_cast<uint8_t>(std::stoi(byte_str, nullptr, 16)));
    }
    return bytes;
  }

  std::string Utils::bytesToString(const std::vector<uint8_t> &bytes)
  {
    return std::string(bytes.begin(), bytes.end());
  }

  std::vector<uint8_t> Utils::stringToBytes(const std::string &data)
  {
    std::vector<uint8_t> bytes;
    bytes.reserve(data.size());
    for (const auto &byte : data)
      bytes.push_back(static_cast<uint8_t>(byte));
    return bytes;
  }

  std::string Utils::intToHex(int64_t value)
  {
    std::stringstream ss;
    ss << std::hex << value;
    return ss.str();
  }
}
