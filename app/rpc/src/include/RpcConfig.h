#ifndef RPC_CONFIG_H
#define RPC_CONFIG_H

#include <unordered_map>
#include <string>

/**
 * Rpc server config: ip, port
 * Zoonkeeper config: ip, port
 */
class RpcConfig
{
public:
  RpcConfig();
  void loadConfigFile(const std::string &config_file_name);
  std::string getConfig(const std::string &key);

private:
  RpcConfig(const RpcConfig &) = delete;
  RpcConfig(RpcConfig &&) = delete;

  std::unordered_map<std::string, std::string> config_map;
};

#endif
