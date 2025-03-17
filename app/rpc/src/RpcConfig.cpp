#include "RpcConfig.h"
#include <iostream>
#include "Logger.h"
#include <cstring>

RpcConfig::RpcConfig()
{
}

void RpcConfig::loadConfigFile(const std::string &config_file_name)
{
  FILE *pf = fopen(config_file_name.c_str(), "r");
  if (pf == nullptr)
  {
    LOG_FATAL("loadConfigFile error: %s", config_file_name.c_str());
    exit(-1);
  }

  while (!feof(pf))
  {
    char buf[512] = {0};
    if (fgets(buf, 512, pf) == nullptr)
    {
      break;
    }

    // Skip empty lines
    if (strlen(buf) == 0)
      continue;

    // Remove spaces and newlines
    std::string line;
    for (char *p = buf; *p != '\0'; ++p)
    {
      if (*p != ' ' && *p != '\n' && *p != '\r')
        line += *p;
    }

    // Skip empty lines after trimming
    if (line.empty())
      continue;

    // Skip comments
    if (line[0] == '#')
      continue;

    // Parse key-value pair
    size_t idx = line.find('=');
    if (idx == std::string::npos)
      continue;

    std::string key = line.substr(0, idx);
    std::string value = line.substr(idx + 1);

    if (!key.empty() && !value.empty())
    {
      config_map[key] = value;
    }
  }

  fclose(pf);
}

std::string RpcConfig::getConfig(const std::string &key)
{
  auto it = config_map.find(key);
  if (it == config_map.end())
  {
    LOG_FATAL("getConfig error: %s", key.c_str());
    exit(-1);
  }
  return it->second;
}
