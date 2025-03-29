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

    Wallet wallet1 = Wallet::createWallet("408D58C4E0371A515A276BC783D099751056C5724F98FC267DFEB6024E7A7218");
    Wallet wallet2 = Wallet::createWallet("9F804E36BB6FCE5F7AFCCEBD5A525922C7F60D601984C515F3F49C7909EC53A3");
    Wallet wallet3 = Wallet::createWallet("13CED1E5E1EBE596D39A1278E016B12D55542791B5BEB7BC3FF0CD274653D8C9");
    Wallet wallet4 = Wallet::createWallet("E36548ECA63AC88F48D772A00ED261A630C850155CE8A2E3208A66EE50161F4A");
    Wallet wallet5 = Wallet::createWallet("E9063CCF773A7B9D0AC1C2EC0AC3B0CC2257F51C9A2951CC15AC52175703AEB2");
    Wallet wallet6 = Wallet::createWallet("2AF12CD01D157CC2B135E0E843696F6309A9BDBF84FEDABCF492ABCF2247652E");

    std::string command;
    std::cout << "Usage: " << std::endl;
    // std::cout << "add <data> - add a block to the blockchain" << std::endl;
    // std::cout << "example: add: Hello, World!" << std::endl;
    std::cout << "print - print the blockchain" << std::endl;
    std::cout << "wallets - print all local wallets" << std::endl;
    std::cout << "balance <address> - get the balance of an address" << std::endl;
    // std::cout << "example: balance: 1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa" << std::endl;
    std::cout << "send - send a transaction" << std::endl;
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
        std::cout << "choose your sender wallet:" << std::endl;
        std::cout << "1. wallet1: " << wallet1.publicKeyInHex() << std::endl;
        std::cout << "2. wallet2: " << wallet2.publicKeyInHex() << std::endl;
        std::cout << "3. wallet3: " << wallet3.publicKeyInHex() << std::endl;
        std::cout << "4. wallet4: " << wallet4.publicKeyInHex() << std::endl;
        std::cout << "5. wallet5: " << wallet5.publicKeyInHex() << std::endl;
        std::cout << "6. wallet6: " << wallet6.publicKeyInHex() << std::endl;
        int choice;
        std::cin >> choice;
        Wallet sender_wallet = (choice == 1) ? wallet1 : (choice == 2) ? wallet2
                                                     : (choice == 3)   ? wallet3
                                                     : (choice == 4)   ? wallet4
                                                     : (choice == 5)   ? wallet5
                                                                       : wallet6;
        std::cout << "choose your receiver wallet:" << std::endl;
        std::cout << "1. wallet1: " << wallet1.publicKeyInHex() << ", balance: " << blockchain_.getAddressBalance(wallet1.publicKeyInHex()) << std::endl;
        std::cout << "2. wallet2: " << wallet2.publicKeyInHex() << ", balance: " << blockchain_.getAddressBalance(wallet2.publicKeyInHex()) << std::endl;
        std::cout << "3. wallet3: " << wallet3.publicKeyInHex() << ", balance: " << blockchain_.getAddressBalance(wallet3.publicKeyInHex()) << std::endl;
        std::cout << "4. wallet4: " << wallet4.publicKeyInHex() << ", balance: " << blockchain_.getAddressBalance(wallet4.publicKeyInHex()) << std::endl;
        std::cout << "5. wallet5: " << wallet5.publicKeyInHex() << ", balance: " << blockchain_.getAddressBalance(wallet5.publicKeyInHex()) << std::endl;
        std::cout << "6. wallet6: " << wallet6.publicKeyInHex() << ", balance: " << blockchain_.getAddressBalance(wallet6.publicKeyInHex()) << std::endl;
        std::cin >> choice;
        Wallet receiver_wallet = (choice == 1) ? wallet1 : (choice == 2) ? wallet2
                                                       : (choice == 3)   ? wallet3
                                                       : (choice == 4)   ? wallet4
                                                       : (choice == 5)   ? wallet5
                                                                         : wallet6;
        std::cout << "enter the amount you want to send:" << std::endl;
        std::string amount;
        std::cin >> amount;

        std::optional<Transaction> transaction = blockchain_.createTransaction(sender_wallet.publicKeyInHex(), receiver_wallet.publicKeyInHex(), amount, sender_wallet);
        if (transaction.has_value())
        {
          blockchain_.addBlock(std::vector<Transaction>({transaction.value()}));
          std::cout << "Transaction sent successfully, the transaction id is: " << transaction.value().tx_id().toHex() << std::endl;
          std::cout << "The block hash is: " << blockchain_.lastBlock().hash().toHex() << std::endl;
        }
        else
        {
          std::cout << "Transaction failed, because of insufficient balance" << std::endl;
        }
      }
      else if (command == "wallets")
      {
        std::cout << "Wallets:" << std::endl;
        std::cout << "1. wallet1: " << wallet1.publicKeyInHex() << std::endl;
        std::cout << "2. wallet2: " << wallet2.publicKeyInHex() << std::endl;
        std::cout << "3. wallet3: " << wallet3.publicKeyInHex() << std::endl;
        std::cout << "4. wallet4: " << wallet4.publicKeyInHex() << std::endl;
        std::cout << "5. wallet5: " << wallet5.publicKeyInHex() << std::endl;
        std::cout << "6. wallet6: " << wallet6.publicKeyInHex() << std::endl;
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