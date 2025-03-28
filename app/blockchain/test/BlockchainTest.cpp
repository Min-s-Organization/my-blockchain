#include "Blockchain.h"
#include <iostream>

using namespace block_chain;

int main()
{
  std::cout << "\nBlockchainTest running..." << std::endl;

  Blockchain blockchain = Blockchain::createBlockchain(Blockchain::BLOCK_CHAIN_DB_NAME);

  blockchain.addBlock(std::vector<Transaction>());
  blockchain.addBlock(std::vector<Transaction>());
  blockchain.addBlock(std::vector<Transaction>());

  blockchain.print();

  return 0;
}