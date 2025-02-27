#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Utils
{
public:
  static std::string bytesToHexString(const std::vector<uint8_t> &bytes);

private:
};