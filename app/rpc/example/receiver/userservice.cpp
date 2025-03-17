#include <string>
#include <iostream>
#include "user.pb.h"
#include "RpcApplication.h"
#include "RpcProvider.h"

class UserService : public example::UserServiceRpc
{
public:
  bool Login(std::string name, std::string pwd)
  {
    std::cout << "call service Login" << std::endl;
    std::cout << "name: " << name << ", pwd: " << pwd << std::endl;
    return true;
  }

  void Login(::google::protobuf::RpcController *controller,
             const ::example::LoginRequest *request,
             ::example::LoginResponse *response,
             ::google::protobuf::Closure *done)
  {
    std::string name = request->name();
    std::string pwd = request->pwd();

    // local service
    bool login_result = Login(name, pwd);

    example::ResultCode *code = response->mutable_result();
    code->set_code(1);
    code->set_msg("success");
    response->set_success(true);

    done->Run();
  }
};

int main(int argc, char **argv)
{
  UserService userservice;

  RpcApplication::init(argc, argv);
  RpcProvider provider;
  provider.notifyService(&userservice);
  provider.run();

  return 0;
}
