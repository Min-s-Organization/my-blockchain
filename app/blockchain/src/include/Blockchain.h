#pragma once

#include "Block.h"
#include <vector>

namespace block_chain
{
  class Blockchain
  {
  public:
    Blockchain() : blocks_() {}
    ~Blockchain() = default;
    void addBlock(std::string data);
    const Block &lastBlock() const;
    void print() const;

  private:
    std::vector<std::unique_ptr<Block>> blocks_;
  };
};