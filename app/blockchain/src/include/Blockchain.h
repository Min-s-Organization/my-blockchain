#pragma once

#include "Block.h"
#include <vector>
#include <memory>
#include <leveldb/db.h>

namespace block_chain
{
  class Blockchain
  {
  public:
    static constexpr const char *LAST_BLOCK_KEY = "LAST_BLOCK";
    static constexpr const char *BLOCK_CHAIN_DB_NAME = "../blockchain.db";

    Blockchain(leveldb::DB *db);
    Blockchain(std::unique_ptr<Block> last_block, leveldb::DB *db);

    static void createBlockchain(std::string db_name, std::unique_ptr<Blockchain> &blockchain);

    ~Blockchain() { delete db_; }
    void addBlock(std::string data);
    const Block &lastBlock() const;
    void print() const;

  private:
    std::unique_ptr<Block> last_block_;
    leveldb::DB *db_;
  };
};