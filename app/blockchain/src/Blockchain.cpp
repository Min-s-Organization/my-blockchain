#include "Blockchain.h"
#include <iostream>
#include "Utils.h"

namespace block_chain
{
  void Blockchain::addBlock(std::string data)
  {
    if (blocks_.empty())
    {
      blocks_.push_back(Block(Utils::stringToBytes(data), Hash()));
    }
    else
    {
      blocks_.push_back(Block(Utils::stringToBytes(data), blocks_.back().getHash()));
    }
  }

  const Block &Blockchain::getLatestBlock() const
  {
    return blocks_.back();
  }

  void Blockchain::print() const
  {
    for (const auto &block : blocks_)
    {
      block.print();
      std::cout << "--------------------------------" << std::endl;
    }
  }
}