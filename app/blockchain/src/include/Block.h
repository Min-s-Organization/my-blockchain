#pragma once

#include <vector>
#include <cstdint>
#include <ctime>
#include <openssl/ssl.h>
#include "Hash.h"
#include <memory>

namespace block_chain
{
  class Block
  {
  public:
    Block(const std::vector<uint8_t> &data, const Hash &prev_hash);
    Block() = default;
    ~Block() = default;

    const Hash &hash() const { return *hash_; };
    void setHash(const Hash &hash) { hash_ = std::make_unique<Hash>(hash); }

    const std::vector<uint8_t> &data() const { return data_; }
    const Hash &prev_hash() const { return *prev_hash_; }

    const time_t &timestamp() const { return timestamp_; }
    std::vector<uint8_t> timestampToBytes() const
    {
      return std::vector<uint8_t>(reinterpret_cast<const uint8_t *>(&timestamp_),
                                  reinterpret_cast<const uint8_t *>(&timestamp_) + sizeof(time_t));
    }

    void setNonce(int64_t nonce) { nonce_ = nonce; }
    int64_t nonce() const { return nonce_; }

    void print() const;

    static void serialize(const Block &block, std::vector<uint8_t> &data);
    static void deserialize(const std::vector<uint8_t> &data, Block &block);

  private:
    time_t timestamp_;
    std::vector<uint8_t> data_;
    std::unique_ptr<Hash> hash_;
    std::unique_ptr<Hash> prev_hash_;
    int64_t nonce_;
    bool is_valid_;
  };
}