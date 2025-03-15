#include "Blockchain.h"
#include <iostream>

using namespace block_chain;

int main()
{
  std::cout << "\nBlockchainTest running..." << std::endl;

  std::unique_ptr<Blockchain> blockchain;
  Blockchain::createBlockchain(Blockchain::BLOCK_CHAIN_DB_NAME, blockchain);

  blockchain->addBlock("Genesis Block");
  blockchain->addBlock("transaction 1");
  blockchain->addBlock("transaction 2");
  blockchain->addBlock("transaction 3");

  blockchain->print();

  return 0;
}