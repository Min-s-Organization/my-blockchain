#include "RpcApplication.h"
#include <unistd.h>
#include "Logger.h"

RpcApplication::RpcApplication()
{
}

RpcConfig RpcApplication::rpc_config_;

RpcApplication &RpcApplication::getInstance()
{
  static RpcApplication instance;
  return instance;
}

void RpcApplication::init(int argc, char **argv)
{
  if (argc < 2)
  {
    LOG_FATAL("usage: command -i <config_file>");
    exit(-1);
  }

  // parse command line arguments
  int c = 0;
  std::string config_file;
  while ((c = getopt(argc, argv, "i:")) != -1)
  {
    switch (c)
    {
    case 'i':
      config_file = optarg;
      break;
    case '?':
      LOG_FATAL("invalid command");
      exit(-1);
    case ':':
      LOG_FATAL("need config file");
      exit(-1);
    default:
      break;
    }
  }

  // load config file
  rpc_config_.loadConfigFile(config_file);

  // LOG_INFO("rpc_server_ip: %s", rpc_config_.getConfig("rpc_server_ip").c_str());
  // LOG_INFO("rpc_server_port: %s", rpc_config_.getConfig("rpc_server_port").c_str());
  // LOG_INFO("zookeeper_ip: %s", rpc_config_.getConfig("zookeeper_ip").c_str());
  // LOG_INFO("zookeeper_port: %s", rpc_config_.getConfig("zookeeper_port").c_str());
}
