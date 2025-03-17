#ifndef RPC_PROVIDER_H
#define RPC_PROVIDER_H

#include <google/protobuf/service.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

class RpcProvider
{
public:
  RpcProvider();
  void notifyService(google::protobuf::Service *service);
  void run();

private:
  std::unique_ptr<muduo::net::TcpServer> server_ptr_;
  muduo::net::EventLoop event_loop_;

  void onConnection(const muduo::net::TcpConnectionPtr &conn);
  void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time);
  void sendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message *response);

  struct ServiceInfo
  {
    google::protobuf::Service *service_;
    std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> method_map_;
  };
  std::unordered_map<std::string, ServiceInfo> service_map_;
};

#endif