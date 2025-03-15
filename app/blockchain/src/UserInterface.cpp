#include "UserInterface.h"

#include <iostream>
#include <memory>

namespace block_chain
{
  UserInterface::UserInterface()
  {
    Blockchain::createBlockchain(Blockchain::BLOCK_CHAIN_DB_NAME, blockchain_);
  }

  void UserInterface::run()
  {
    std::cout << "Welcome to my blockchain!" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::string command;
    std::cout << "Usage: " << std::endl;
    std::cout << "add: <data> - add a block to the blockchain" << std::endl;
    std::cout << "example: add: Hello, World!" << std::endl;
    std::cout << "print - print the blockchain" << std::endl;
    std::cout << "exit - exit the program" << std::endl;
    while (true)
    {
      std::cout << "\nEnter a command: ";
      std::cin >> command;

      if (command == "add")
      {
        std::string data;
        std::getline(std::cin, data);
        blockchain_->addBlock(data);
        blockchain_->lastBlock().print();
        std::cout << "Block added to the blockchain successfully" << std::endl;
      }
      else if (command == "print")
      {
        std::cout << "Printing the blockchain..." << std::endl;
        blockchain_->print();
      }
      else if (command == "exit")
      {
        break;
      }
    }
  }

  void UserInterface::addBlock(const std::string &data)
  {
    blockchain_->addBlock(data);
  }

  void UserInterface::printBlockchain()
  {
    blockchain_->print();
  }
}