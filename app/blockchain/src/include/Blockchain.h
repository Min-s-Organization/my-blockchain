#pragma once

#include "Block.h"
#include "Wallet.h"
#include <vector>
#include <memory>
#include <leveldb/db.h>
#include <optional>

namespace block_chain
{
  class Blockchain
  {
  public:
    static constexpr const char *LAST_BLOCK_KEY = "LAST_BLOCK";
    static constexpr const char *BLOCK_CHAIN_DB_NAME = "../blockchain.db";

    Blockchain(leveldb::DB *db);
    Blockchain(Block last_block, leveldb::DB *db);

    static Blockchain createBlockchain(std::string db_name);

    ~Blockchain() { delete db_; }
    void addBlock(std::vector<Transaction> transactions);
    const Block &lastBlock() const;
    void print() const;
    Block block(const std::string &hash) const;

    std::string getAddressBalance(const std::string &address) const;
    std::optional<Transaction> createTransaction(const std::string &from, const std::string &to, std::string amount, Wallet &wallet);

  private:
    Block last_block_;
    leveldb::DB *db_;

    std::vector<Transaction> findUnspentTransactions(const std::string &address) const;
    std::vector<std::tuple<std::string, size_t, TransactionOutput>> findUnspentTransactionsOutputs(const std::string &address) const;
    std::vector<std::tuple<std::string, size_t, TransactionOutput>> findUnspentTransactionsOutputs(const std::string &address, std::string amount) const;
  };
};