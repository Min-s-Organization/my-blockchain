#pragma once

#include "Blockchain.h"

namespace block_chain
{
  class UserInterface
  {
  public:
    UserInterface();
    ~UserInterface() = default;

    void run();
    void addBlock(const std::string &data);
    void printBlockchain();

  private:
    Blockchain blockchain_;
  };
}