#include "Blockchain.h"
#include <iostream>
#include "Utils.h"
#include "leveldb/db.h"

namespace block_chain
{
  Blockchain::Blockchain(leveldb::DB *db) : last_block_(nullptr), db_(db)
  {
  }

  Blockchain::Blockchain(std::unique_ptr<Block> last_block, leveldb::DB *db) : last_block_(std::move(last_block)), db_(db)
  {
  }

  void Blockchain::createBlockchain(std::string db_name, std::unique_ptr<Blockchain> &blockchain)
  {
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = false;
    leveldb::Status status = leveldb::DB::Open(options, db_name.c_str(), &db);
    if (!status.ok())
    {
      // create database
      options.create_if_missing = true;
      leveldb::Status status = leveldb::DB::Open(options, db_name.c_str(), &db);
      if (!status.ok())
      {
        std::cerr << "Failed to create database: " << status.ToString() << std::endl;
      }
      else
      {
        // create the first block
        blockchain = std::make_unique<Blockchain>(db);
        blockchain->addBlock("Genesis Block");
      }
    }
    else
    {
      // read the last block from the database and save in the new blockchain instance
      std::string last_block_serialized;
      status = db->Get(leveldb::ReadOptions(), LAST_BLOCK_KEY, &last_block_serialized);
      assert(status.ok());

      // create a new blockchain instance
      // and initialize the last block
      auto last_block = std::make_unique<Block>();
      last_block->deserialize(std::vector<uint8_t>(last_block_serialized.begin(), last_block_serialized.end()), *last_block);
      blockchain = std::make_unique<Blockchain>(db);
      blockchain->last_block_ = std::move(last_block);
    }
  }

  void Blockchain::addBlock(std::string data)
  {
    if (last_block_ == nullptr)
    {
      last_block_ = std::make_unique<Block>(Utils::stringToBytes(data), Hash());
    }
    else
    {
      last_block_ = std::make_unique<Block>(Utils::stringToBytes(data), last_block_->hash());
    }

    // save the block in the database
    std::vector<uint8_t> block_serialized;
    last_block_->serialize(*last_block_, block_serialized);
    leveldb::Status status = db_->Put(
        leveldb::WriteOptions(),
        last_block_->hash().toHex(),
        std::string(block_serialized.begin(), block_serialized.end()));
    assert(status.ok());

    // update the last block in the database
    status = db_->Put(
        leveldb::WriteOptions(),
        LAST_BLOCK_KEY,
        std::string(block_serialized.begin(), block_serialized.end()));
    assert(status.ok());
  }

  const Block &Blockchain::lastBlock() const
  {
    return *last_block_;
  }

  void Blockchain::print() const
  {
    leveldb::Iterator *it = db_->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
      std::string key = it->key().ToString();
      std::string value = it->value().ToString();

      Block block;
      block.deserialize(std::vector<uint8_t>(value.begin(), value.end()), block);
      block.print();
      std::cout << "--------------------------------" << std::endl;
    }
    delete it;
  }
}