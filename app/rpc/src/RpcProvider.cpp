#include "RpcProvider.h"
#include "RpcApplication.h"
#include <functional>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "RpcHeader.pb.h"
#include "Logger.h"

RpcProvider::RpcProvider()
{
}

void RpcProvider::notifyService(google::protobuf::Service *service)
{
  ServiceInfo service_info;

  const auto *service_desc = service->GetDescriptor();
  std::string service_name = service_desc->name();
  int method_count = service_desc->method_count();

  service_info.service_ = service;
  for (int i = 0; i < method_count; i++)
  {
    const auto *method_desc = service_desc->method(i);
    service_info.method_map_.insert({method_desc->name(), method_desc});
  }
  service_map_.insert({service_name, service_info});
}

void RpcProvider::run()
{
  std::string rpc_server_ip = RpcApplication::getInstance().getRpcConfig().getConfig("rpc_server_ip");
  uint16_t rpc_server_port = std::stoi(RpcApplication::getInstance().getRpcConfig().getConfig("rpc_server_port"));

  muduo::net::InetAddress address(rpc_server_ip, rpc_server_port);
  server_ptr_ = std::make_unique<muduo::net::TcpServer>(&event_loop_, address, "RpcProvider");

  // Set callback function of connection and message
  server_ptr_->setConnectionCallback(std::bind(&RpcProvider::onConnection, this, std::placeholders::_1));
  server_ptr_->setMessageCallback(std::bind(&RpcProvider::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  server_ptr_->setThreadNum(4);

  server_ptr_->start();
  event_loop_.loop();
}

void RpcProvider::onConnection(const muduo::net::TcpConnectionPtr &conn)
{
  if (!conn->connected())
  {
    conn->shutdown();
  }
}

void RpcProvider::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time)
{
  std::string recv_buf = buffer->retrieveAllAsString();

  uint32_t header_size = 0;
  recv_buf.copy((char *)&header_size, 4, 0);

  std::string rpc_header_str = recv_buf.substr(4, header_size);
  rpc::RpcHeader rpc_header;
  std::string service_name;
  std::string method_name;
  uint32_t args_size = 0;

  // Read header
  if (rpc_header.ParseFromString(rpc_header_str))
  {
    service_name = rpc_header.service_name();
    method_name = rpc_header.method_name();
    args_size = rpc_header.args_size();

    LOG_INFO("service_name: %s", service_name.c_str());
    LOG_INFO("method_name: %s", method_name.c_str());
    LOG_INFO("args_size: %d", args_size);
  }
  else
  {
    LOG_ERROR("Invalid RpcHeader: %s", rpc_header_str.c_str());
    return;
  }

  // Read args
  std::string args_str = recv_buf.substr(4 + header_size, args_size);

  // Get the service and method
  auto service_it = service_map_.find(service_name);
  if (service_it == service_map_.end())
  {
    LOG_ERROR("Service not found: %s", service_name.c_str());
    return;
  }

  auto method_it = service_it->second.method_map_.find(method_name);
  if (method_it == service_it->second.method_map_.end())
  {
    LOG_ERROR("Method not found: %s in service %s", method_name.c_str(), service_name.c_str());
    return;
  }

  auto service = service_it->second.service_;
  auto method = method_it->second;

  auto request = service->GetRequestPrototype(method).New();
  if (!request->ParseFromString(args_str))
  {
    LOG_ERROR("Failed to parse request for method: %s", method_name.c_str());
    delete request;
    return;
  }
  auto response = service->GetResponsePrototype(method).New();

  auto done = google::protobuf::NewCallback<RpcProvider, const muduo::net::TcpConnectionPtr &, google::protobuf::Message *>(this, &RpcProvider::sendRpcResponse, conn, response);
  service->CallMethod(method, nullptr, request, response, done);
  delete request;
}

void RpcProvider::sendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message *response)
{
  std::string response_str;
  if (response->SerializeToString(&response_str))
  {
    conn->send(response_str);
  }
  else
  {
    LOG_ERROR("Failed to serialize response");
  }
  conn->shutdown();
  delete response;
}