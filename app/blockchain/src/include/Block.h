#pragma once

#include <vector>
#include <cstdint>
#include <ctime>
#include <openssl/ssl.h>
#include <memory>
#include "Transaction.h"
#include "Hash.h"

namespace block_chain
{
  class Block
  {
  public:
    Block(const std::vector<Transaction> &transactions, const Hash &prev_hash);
    Block() = default;
    ~Block() = default;

    const Hash &hash() const { return hash_; };
    void setHash(const Hash &hash) { hash_ = hash; }

    const std::vector<Transaction> &transactions() const { return transactions_; }
    const Hash &prev_hash() const { return prev_hash_; }

    const time_t &timestamp() const { return timestamp_; }
    std::vector<uint8_t> timestampToBytes() const
    {
      return std::vector<uint8_t>(reinterpret_cast<const uint8_t *>(&timestamp_),
                                  reinterpret_cast<const uint8_t *>(&timestamp_) + sizeof(time_t));
    }

    void setNonce(int64_t nonce) { nonce_ = nonce; }
    int64_t nonce() const { return nonce_; }

    void print() const;

    static std::vector<uint8_t> serialize(const Block &block);
    static Block deserialize(const std::vector<uint8_t> &data);

  private:
    time_t timestamp_;
    std::vector<Transaction> transactions_;
    Hash hash_;
    Hash prev_hash_;
    int64_t nonce_;
    bool is_valid_;
  };
}