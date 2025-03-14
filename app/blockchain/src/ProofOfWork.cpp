#include "ProofOfWork.h"
#include "Hash.h"
#include <iostream>
#include "Utils.h"
#include <cstring>

namespace block_chain
{
  ProofOfWork::ProofOfWork(Block &block)
      : block_(block), target_(std::make_unique<Hash>(generateTarget()))
  {
  }

  const Hash ProofOfWork::generateTarget() const
  {
    std::vector<uint8_t> target(32, 0xff);
    for (int i = 0; i < DIFFICULTY; i++)
    {
      target[i] = 0x00;
    }
    return Hash(target, true);
  }

  const std::vector<uint8_t> ProofOfWork::prepare(int64_t nonce) const
  {
    std::vector<uint8_t> input;

    const auto &blockData = block_.data();
    for (const auto &byte : blockData)
      input.push_back(byte);

    const auto &prevHash = block_.prev_hash().toBytes();
    for (const auto &byte : prevHash)
      input.push_back(byte);

    const auto &timestamp = block_.timestampToBytes();
    for (const auto &byte : timestamp)
      input.push_back(byte);

    std::string nonceHex = Utils::intToHex(nonce);
    for (const auto &ch : nonceHex)
      input.push_back(static_cast<uint8_t>(ch));

    return input;
  }

  const void ProofOfWork::run()
  {
    int64_t nonce = 0;
    Hash hash;

    while (nonce < MAX_NONCE)
    {
      std::vector<uint8_t> input = prepare(nonce);
      hash = Hash(input);
      if (hash.toBytes() < target_->toBytes())
        break;
      nonce++;
      // std::cout << "nonce: " << nonce << std::endl;
      // std::cout << "hash: " << hash.toHex() << std::endl;
      // std::cout << "target: " << target_->toHex() << std::endl;
    }
    block_.setHash(hash);
    block_.setNonce(nonce);
  }
}