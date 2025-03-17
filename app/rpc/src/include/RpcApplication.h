#ifndef RPC_APPLICATION_H
#define RPC_APPLICATION_H

#include "RpcConfig.h"

class RpcApplication
{
public:
  static void init(int argc, char **argv);
  static RpcApplication &getInstance();
  static RpcConfig &getRpcConfig() { return rpc_config_; }

private:
  static RpcConfig rpc_config_;
  RpcApplication();
  RpcApplication(const RpcApplication &) = delete;
  RpcApplication(RpcApplication &&) = delete;
};

#endif