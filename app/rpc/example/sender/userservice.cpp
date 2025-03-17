#include "user.pb.h"
#include <iostream>
#include "RpcApplication.h"
#include "RpcChannel.h"
#include "Logger.h"

int main(int argc, char **argv)
{
  RpcApplication::init(argc, argv);

  example::UserServiceRpc_Stub stub(new RpcChannel());
  example::LoginRequest request;
  request.set_name("zhangsan");
  request.set_pwd("123456");

  example::LoginResponse response;
  stub.Login(nullptr, &request, &response, nullptr);
  LOG_INFO("Receive response: %s", response.success() ? "success" : "failed");
}