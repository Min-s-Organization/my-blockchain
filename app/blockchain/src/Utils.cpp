#include <Utils.h>
#include <iomanip>

std::string Utils::bytesToHexString(const std::vector<uint8_t> &bytes)
{
  std::stringstream ss;
  ss << std::hex << std::setfill('0');

  for (const auto &byte : bytes)
  {
    ss << std::setw(2) << static_cast<int>(byte);
  }

  return ss.str();
}