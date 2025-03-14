#pragma once

#include "Hash.h"
#include "Block.h"
#include <vector>

namespace block_chain
{
  const int64_t MAX_NONCE = INT64_MAX;
  const int DIFFICULTY = 2;

  class ProofOfWork
  {
  public:
    ProofOfWork(Block &block);
    const void run();

  private:
    Block &block_;
    std::unique_ptr<Hash> target_;

    const std::vector<uint8_t> prepare(int64_t nonce) const;

    const Hash generateTarget() const;
  };
}
