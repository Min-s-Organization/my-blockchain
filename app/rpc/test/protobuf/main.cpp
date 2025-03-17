#include "test.pb.h"
#include <iostream>

using namespace test;

int main()
{
  LoginRequest req;
  req.set_name("zhimin");
  req.set_pwd("0905");

  std::string send_str;
  if (req.SerializeToString(&send_str))
  {
    std::cout << send_str << std::endl;
  }
  else
  {
    std::cout << "fail to serialize" << std::endl;
  }

  return 0;
}