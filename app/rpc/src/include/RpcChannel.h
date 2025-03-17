#ifndef RPC_CHANNEL_H
#define RPC_CHANNEL_H

#include "RpcApplication.h"
#include <google/protobuf/service.h>
#include <muduo/net/TcpConnection.h>

class RpcChannel : public google::protobuf::RpcChannel
{
public:
  RpcChannel() = default;
  void CallMethod(const google::protobuf::MethodDescriptor *method,
                  google::protobuf::RpcController *controller,
                  const google::protobuf::Message *request,
                  google::protobuf::Message *response,
                  google::protobuf::Closure *done);
};

#endif
