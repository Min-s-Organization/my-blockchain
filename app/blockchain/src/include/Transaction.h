#include <string>
#include <vector>
#include <cstdint>
#include "Hash.h"

namespace block_chain
{
  class TransactionOutput
  {
  public:
    TransactionOutput(std::string address, std::string amount) : script_pubkey_(address), amount_(amount) {}
    ~TransactionOutput() = default;

    const std::string &address() const { return script_pubkey_; }
    const std::string &amount() const { return amount_; }
    const bool canUnlock(const std::string &data) const { return script_pubkey_ == data; }

    static std::vector<uint8_t> serialize(const TransactionOutput &output);
    static TransactionOutput deserialize(const std::vector<uint8_t> &data);

    std::string toString() const;

  private:
    std::string amount_;
    std::string script_pubkey_;
  };

  class TransactionInput
  {
  public:
    TransactionInput(std::string tx_id, size_t vout, std::string script_sig) : tx_id_(tx_id), vout_(vout), script_sig_(script_sig) {}
    ~TransactionInput() = default;

    const std::string &tx_id() const { return tx_id_; }
    const size_t &vout() const { return vout_; }
    const std::string &script_sig() const { return script_sig_; }
    const bool canUnlock(const std::string &data) const { return script_sig_ == data; }

    static std::vector<uint8_t> serialize(const TransactionInput &input);
    static TransactionInput deserialize(const std::vector<uint8_t> &data);

    std::string toString() const;

  private:
    std::string tx_id_;
    size_t vout_;
    std::string script_sig_;
  };

  class Transaction
  {
  public:
    static Transaction createCoinbaseTransaction(std::string to_address, std::string amount);

    Transaction(std::vector<TransactionInput> inputs, std::vector<TransactionOutput> outputs) : inputs_(inputs), outputs_(outputs)
    {
      std::vector<uint8_t> data;
      for (const auto &input : inputs)
      {
        std::vector<uint8_t> input_data = TransactionInput::serialize(input);
        data.insert(data.end(), input_data.begin(), input_data.end());
      }
      for (const auto &output : outputs)
      {
        std::vector<uint8_t> output_data = TransactionOutput::serialize(output);
        data.insert(data.end(), output_data.begin(), output_data.end());
      }
      tx_id_ = Hash(data);
    }
    Transaction(std::string tx_id, std::vector<TransactionInput> inputs, std::vector<TransactionOutput> outputs) : tx_id_(Hash(tx_id)), inputs_(inputs), outputs_(outputs) {}
    ~Transaction() = default;

    const Hash &tx_id() const { return tx_id_; }
    const std::vector<TransactionOutput> &outputs() const { return outputs_; }
    const std::vector<TransactionInput> &inputs() const { return inputs_; }

    static std::vector<uint8_t> serialize(const Transaction &transaction);
    static Transaction deserialize(const std::vector<uint8_t> &data);

    std::string toString() const;

  private:
    Hash tx_id_;
    std::vector<TransactionOutput> outputs_;
    std::vector<TransactionInput> inputs_;
  };
}