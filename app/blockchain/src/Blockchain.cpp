#include "Blockchain.h"
#include "Utils.h"
#include "leveldb/db.h"
#include <map>
#include <iostream>

namespace block_chain
{
  Blockchain::Blockchain(leveldb::DB *db) : last_block_(Block()), db_(db)
  {
  }

  Blockchain::Blockchain(Block last_block, leveldb::DB *db) : last_block_(last_block), db_(db)
  {
  }

  Blockchain Blockchain::createBlockchain(std::string db_name)
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
        assert(false);
        return nullptr;
      }
      else
      {
        // create the first block
        Blockchain blockchain(db);
        Transaction coinbase_transaction = Transaction::createCoinbaseTransaction("0363AB2AD907F806783EB46047DC0739C76DD43FB1E6029E48C0700F947BDA8EE3", "10");
        blockchain.addBlock(std::vector<Transaction>({coinbase_transaction}));
        return blockchain;
      }
    }
    else
    {
      // read the last block from the database and save in the new blockchain instance
      std::string last_block_serialized;
      leveldb::Status status = db->Get(leveldb::ReadOptions(), LAST_BLOCK_KEY, &last_block_serialized);
      assert(status.ok());

      // create a new blockchain instance
      // and initialize the last block
      Block last_block = Block::deserialize(std::vector<uint8_t>(last_block_serialized.begin(), last_block_serialized.end()));
      return Blockchain(last_block, db);
    }
    assert(false);
    return nullptr;
  }

  void Blockchain::addBlock(std::vector<Transaction> transactions)
  {
    if (last_block_.hash().isEmpty())
    {
      last_block_ = Block(transactions, Hash());
    }
    else
    {
      last_block_ = Block(transactions, last_block_.hash());
    }

    // save the block in the database
    std::vector<uint8_t> block_serialized;
    block_serialized = Block::serialize(last_block_);
    leveldb::Status status = db_->Put(
        leveldb::WriteOptions(),
        last_block_.hash().toHex(),
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
    return last_block_;
  }

  void Blockchain::print() const
  {
    last_block_.print();
    Hash currentHash = last_block_.prev_hash();
    while (!currentHash.isEmpty())
    {
      std::cout << "--------------------------------" << std::endl;
      std::string current_block_serialized;
      if (currentHash.isEmpty())
      {
        break;
      }
      leveldb::Status status = db_->Get(leveldb::ReadOptions(), currentHash.toHex(), &current_block_serialized);
      if (!status.ok())
      {
        break;
      }

      Block current_block = Block::deserialize(std::vector<uint8_t>(current_block_serialized.begin(), current_block_serialized.end()));
      current_block.print();
      currentHash = current_block.prev_hash();
    };
  }

  Block Blockchain::block(const std::string &hash) const
  {
    std::string block_serialized;
    assert(db_ != nullptr);
    // assert(hash.empty() == false);
    leveldb::Status status = db_->Get(leveldb::ReadOptions(), hash, &block_serialized);
    if (!status.ok())
    {
      return Block();
    }

    return Block::deserialize(std::vector<uint8_t>(block_serialized.begin(), block_serialized.end()));
  }

  std::vector<Transaction> Blockchain::findUnspentTransactions(const std::string &address) const
  {
    std::vector<Transaction> unspent_transactions;
    // key: tx_id, value: vector of output indices that are spent
    std::map<std::string, std::vector<size_t>> spent_transactions;
    Block current_block = last_block_;
    do
    {
      for (const auto &transaction : current_block.transactions())
      {
        // check if there is an input that could be unlocked in the transaction
        for (const auto &input : transaction.inputs())
        {
          if (input.canUnlock(address))
          {
            spent_transactions[input.tx_id()].push_back(input.vout());
          }
        }
        for (int i = 0; i < transaction.outputs().size(); i++)
        {
          auto output = transaction.outputs()[i];
          if (output.canUnlock(address))
          {
            // the entire transaction is unspent
            if (spent_transactions.find(transaction.tx_id().toHex()) == spent_transactions.end())
            {
              unspent_transactions.push_back(transaction);
              break;
            }
            else
            {
              // the transaction is partially spent
              // check if the output is unspent by the index in the inputs
              if (std::find(spent_transactions[transaction.tx_id().toHex()].begin(), spent_transactions[transaction.tx_id().toHex()].end(), i) == spent_transactions[transaction.tx_id().toHex()].end())
              {
                unspent_transactions.push_back(transaction);
                break;
              }
            }
          }
        }
      }
      current_block = block(current_block.prev_hash().toHex());
    } while (!current_block.prev_hash().isEmpty());

    return unspent_transactions;
  }

  std::string Blockchain::getAddressBalance(const std::string &address) const
  {
    std::vector<std::tuple<std::string, size_t, TransactionOutput>> unspent_transactions_outputs = findUnspentTransactionsOutputs(address);
    double balance = 0;
    for (const auto &output : unspent_transactions_outputs)
    {
      balance += std::stod(std::get<2>(output).amount());
    }
    return std::to_string(balance);
  }

  std::vector<std::tuple<std::string, size_t, TransactionOutput>> Blockchain::findUnspentTransactionsOutputs(const std::string &address) const
  {
    std::vector<std::tuple<std::string, size_t, TransactionOutput>> unspent_transactions_outputs;
    std::vector<Transaction> unspent_transactions = findUnspentTransactions(address);

    for (const auto &transaction : unspent_transactions)
    {
      for (size_t i = 0; i < transaction.outputs().size(); i++)
      {
        if (transaction.outputs()[i].canUnlock(address))
        {
          unspent_transactions_outputs.push_back(std::tuple<std::string, size_t, TransactionOutput>(transaction.tx_id().toHex(), i, transaction.outputs()[i]));
        }
      }
    }

    return unspent_transactions_outputs;
  }

  std::vector<std::tuple<std::string, size_t, TransactionOutput>> Blockchain::findUnspentTransactionsOutputs(const std::string &address, std::string amount) const
  {
    std::vector<std::tuple<std::string, size_t, TransactionOutput>> unspent_transactions_outputs = findUnspentTransactionsOutputs(address);
    std::vector<std::tuple<std::string, size_t, TransactionOutput>> outputs;
    double amount_to_spend = std::stod(amount);
    for (const auto &output : unspent_transactions_outputs)
    {
      amount_to_spend -= std::stod(std::get<2>(output).amount());
      outputs.push_back(output);
      if (amount_to_spend <= 0)
      {
        break;
      }
    }
    if (amount_to_spend > 0)
    {
      return std::vector<std::tuple<std::string, size_t, TransactionOutput>>();
    }
    return outputs;
  }

  std::optional<Transaction> Blockchain::createTransaction(const std::string &from, const std::string &to, std::string amount, Wallet &wallet)
  {
    // build the input list
    std::vector<TransactionInput> inputs;
    std::vector<std::tuple<std::string, size_t, TransactionOutput>> unspent_transactions_outputs = findUnspentTransactionsOutputs(from, amount);

    if (unspent_transactions_outputs.empty())
    {
      return std::nullopt;
    }

    for (const auto &output : unspent_transactions_outputs)
    {
      // TODO: get the signature
      TransactionInput input(std::get<0>(output), std::get<1>(output), std::get<2>(output).public_key());
      std::string message = input.message();
      input.sign(Utils::bytesToHex(wallet.sign(message)));
      inputs.push_back(input);
    }

    double total_amount = 0;
    for (const auto &output : unspent_transactions_outputs)
    {
      total_amount += std::stod(std::get<2>(output).amount());
    }

    // build the output list
    std::vector<TransactionOutput> outputs;
    double remaining_amount = total_amount - std::stod(amount);
    outputs.push_back(TransactionOutput(to, amount));
    assert(remaining_amount >= 0);
    // if there is a remaining amount, save it in the from address
    outputs.push_back(TransactionOutput(from, std::to_string(remaining_amount)));

    // create the transaction
    return Transaction(inputs, outputs);
  }
}
