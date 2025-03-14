#include "Blockchain.h"
#include <iostream>

using namespace block_chain;

int main()
{
  std::cout << "\nBlockchainTest running..." << std::endl;

  Blockchain blockchain;

  blockchain.addBlock("Genesis Block");
  blockchain.addBlock("transaction 1");
  blockchain.addBlock("transaction 2");
  blockchain.addBlock("transaction 3");

  blockchain.print();

  return 0;
}