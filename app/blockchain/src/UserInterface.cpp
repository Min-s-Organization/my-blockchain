#include "UserInterface.h"

#include <iostream>
#include <memory>

namespace block_chain
{
  UserInterface::UserInterface() : blockchain_(Blockchain::createBlockchain(Blockchain::BLOCK_CHAIN_DB_NAME))
  {
  }

  void UserInterface::run()
  {
    std::cout << "Welcome to my blockchain!" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::string command;
    std::cout << "Usage: " << std::endl;
    // std::cout << "add <data> - add a block to the blockchain" << std::endl;
    // std::cout << "example: add: Hello, World!" << std::endl;
    std::cout << "print - print the blockchain" << std::endl;
    std::cout << "balance <address> - get the balance of an address" << std::endl;
    // std::cout << "example: balance: 1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa" << std::endl;
    std::cout << "send <amount> <from_address> <to_address> - send a transaction" << std::endl;
    std::cout << "exit - exit the program" << std::endl;
    while (true)
    {
      std::cout << "\nEnter a command: ";
      std::cin >> command;

      // if (command == "add")
      // {
      //   std::string data;
      //   std::getline(std::cin, data);
      //   blockchain_.addBlock(std::vector<Transaction>());
      //   blockchain_.lastBlock().print();
      //   std::cout << "Block added to the blockchain successfully" << std::endl;
      // }
      if (command == "print")
      {
        std::cout << "Printing the blockchain..." << std::endl;
        blockchain_.print();
      }
      else if (command == "exit")
      {
        break;
      }
      else if (command == "balance")
      {
        std::string address;
        std::getline(std::cin, address);
        // remove the space
        address.erase(std::remove(address.begin(), address.end(), ' '), address.end());
        std::cout << "Balance of address " << address << ": " << blockchain_.getAddressBalance(address) << std::endl;
      }
      else if (command == "send")
      {
        std::string amount;
        std::cin >> amount;
        std::string from_address;
        std::cin >> from_address;
        std::string to_address;
        std::cin >> to_address;
        std::optional<Transaction> transaction = blockchain_.createTransaction(from_address, to_address, amount);
        if (transaction.has_value())
        {
          blockchain_.addBlock(std::vector<Transaction>({transaction.value()}));
          std::cout << "Transaction sent successfully" << std::endl;
        }
        else
        {
          std::cout << "Transaction failed" << std::endl;
        }
      }
    }
  }

  void UserInterface::addBlock(const std::string &data)
  {
    blockchain_.addBlock(std::vector<Transaction>());
  }

  void UserInterface::printBlockchain()
  {
    blockchain_.print();
  }
}