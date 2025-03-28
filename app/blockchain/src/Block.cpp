#include "Block.h"
#include <iostream>
#include "Utils.h"
#include <vector>
#include "ProofOfWork.h"
#include "block.pb.h"

namespace block_chain
{
  Block::Block(const std::vector<Transaction> &transactions, const Hash &prev_hash)
      : transactions_(transactions), prev_hash_(Hash(prev_hash))
  {
    timestamp_ = time(nullptr);
    //     std::vector<const uint8_t *> input;
    //     input.push_back(data.data());
    //
    //     if (prev_hash.toBytes().size() > 0)
    //       input.push_back(prev_hash.toBytes().data());
    //
    //     input.push_back(reinterpret_cast<const uint8_t *>(&timestamp_));

    ProofOfWork pow(*this);
    pow.run();
    is_valid_ = pow.isValid();
  }

  void Block::print() const
  {
    std::cout << "Block hash: " << hash_.toHex() << std::endl;
    std::cout << "Block prev hash: " << prev_hash_.toHex() << std::endl;
    std::cout << "Block timestamp: " << timestamp_ << std::endl;
    std::cout << "Block nonce: " << nonce_ << std::endl;
    std::cout << "Block is valid: " << is_valid_ << std::endl;
    // print transactions
    for (const auto &transaction : transactions_)
    {
      std::cout << "Block transactions: " << transaction.toString() << std::endl;
    }
  }

  std::vector<uint8_t> Block::serialize(const Block &block)
  {
    BlockProto block_proto;

    for (const auto &transaction : block.transactions_)
    {
      TransactionProto *transaction_proto = block_proto.add_transactions();
      std::vector<uint8_t> serialized_data = Transaction::serialize(transaction);
      transaction_proto->ParseFromArray(serialized_data.data(), serialized_data.size());
    }
    block_proto.set_timestamp(block.timestamp_);
    block_proto.set_hash(Utils::bytesToString(block.hash_.toBytes()));
    block_proto.set_prev_hash(Utils::bytesToString(block.prev_hash_.toBytes()));
    block_proto.set_nonce(block.nonce_);
    block_proto.set_is_valid(block.is_valid_);

    std::string serialized_data;
    block_proto.SerializeToString(&serialized_data);
    return std::vector<uint8_t>(serialized_data.begin(), serialized_data.end());
  }

  Block Block::deserialize(const std::vector<uint8_t> &data)
  {
    Block block;
    BlockProto block_proto;
    std::string serialized_data(data.begin(), data.end());
    block_proto.ParseFromString(serialized_data);
    for (const auto &transaction_proto : block_proto.transactions())
    {
      std::string serialized_string = transaction_proto.SerializeAsString();
      std::vector<uint8_t> serialized_data(serialized_string.begin(), serialized_string.end());
      Transaction transaction = Transaction::deserialize(serialized_data);
      block.transactions_.push_back(transaction);
    }

    block.timestamp_ = block_proto.timestamp();

    std::vector<uint8_t> hash_bytes(block_proto.hash().begin(), block_proto.hash().end());
    Hash hash(hash_bytes, true);
    block.hash_ = hash;

    std::vector<uint8_t> prev_hash_bytes(block_proto.prev_hash().begin(), block_proto.prev_hash().end());
    Hash prev_hash(prev_hash_bytes, true);
    block.prev_hash_ = prev_hash;

    block.nonce_ = block_proto.nonce();
    block.is_valid_ = block_proto.is_valid();

    return block;
  }
}