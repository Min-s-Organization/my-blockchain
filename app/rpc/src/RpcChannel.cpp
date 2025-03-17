#include "RpcChannel.h"
#include "RpcHeader.pb.h"
#include "Logger.h"
#include <string>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <arpa/inet.h>

void RpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                            google::protobuf::RpcController *controller,
                            const google::protobuf::Message *request,
                            google::protobuf::Message *response,
                            google::protobuf::Closure *done)
{
  const google::protobuf::ServiceDescriptor *sd = method->service();
  std::string service_name = sd->name();
  std::string method_name = method->name();

  int args_size = 0;
  std::string args_str;
  if (request->SerializeToString(&args_str))
  {
    args_size = args_str.size();
  }
  else
  {
    LOG_ERROR("Serialize request error!");
    return;
  }

  rpc::RpcHeader rpc_header;
  rpc_header.set_service_name(service_name);
  rpc_header.set_method_name(method_name);
  rpc_header.set_args_size(args_size);

  uint32_t header_size = 0;
  std::string rpc_header_str;
  if (!rpc_header.SerializeToString(&rpc_header_str))
  {
    LOG_ERROR("Serialize rpc header error!");
    return;
  }
  header_size = rpc_header_str.size();

  std::string send_rpc_str;
  send_rpc_str.insert(0, std::string((char *)&header_size, 4));
  send_rpc_str += rpc_header_str;
  send_rpc_str += args_str;

  LOG_INFO("Send rpc string: %s", send_rpc_str.c_str());

  // send rpc_str via socket programming
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    LOG_ERROR("Create socket error!");
    return;
  }
  std::string ip = RpcApplication::getInstance().getRpcConfig().getConfig("rpc_server_ip");
  uint16_t port = atoi(RpcApplication::getInstance().getRpcConfig().getConfig("rpc_server_port").c_str());

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    LOG_FATAL("Connect error!");
    close(sockfd);
    exit(-1);
  }

  if (send(sockfd, send_rpc_str.c_str(), send_rpc_str.size(), 0) < 0)
  {
    LOG_ERROR("Send error!");
    close(sockfd);
    return;
  }

  char buffer[1024] = {0};
  int recv_size = 0;
  if ((recv_size = recv(sockfd, buffer, sizeof(buffer), 0)) < 0)
  {
    LOG_ERROR("Receive error!");
    close(sockfd);
    return;
  }

  if (!response->ParseFromArray(buffer, recv_size))
  {
    LOG_ERROR("Parse response error! Response string: %s", buffer);
    close(sockfd);
    return;
  }

  close(sockfd);
}