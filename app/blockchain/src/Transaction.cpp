#include "Transaction.h"
#include "block.pb.h"
#include <string>
#include <sstream>

namespace block_chain
{
  Transaction Transaction::createCoinbaseTransaction(std::string to_address, std::string data)
  {
    return Transaction(
        {TransactionInput("", -1, data)},
        // TODO: change the amount to the actual amount of the coinbase transaction
        {TransactionOutput(to_address, "10")});
  }

  std::vector<uint8_t> Transaction::serialize(const Transaction &transaction)
  {
    TransactionProto transaction_proto;
    transaction_proto.set_tx_id(transaction.tx_id_.toHex());

    // Serialize inputs
    for (const auto &input : transaction.inputs_)
    {
      TransactionInputProto *input_proto = transaction_proto.add_inputs();
      std::vector<uint8_t> serialized_data = TransactionInput::serialize(input);
      input_proto->ParseFromArray(serialized_data.data(), serialized_data.size());
    }

    // Serialize outputs
    for (const auto &output : transaction.outputs_)
    {
      TransactionOutputProto *output_proto = transaction_proto.add_outputs();
      std::vector<uint8_t> serialized_data = TransactionOutput::serialize(output);
      output_proto->ParseFromArray(serialized_data.data(), serialized_data.size());
    }

    std::string serialized_data;
    transaction_proto.SerializeToString(&serialized_data);

    return std::vector<uint8_t>(serialized_data.begin(), serialized_data.end());
  }

  Transaction Transaction::deserialize(const std::vector<uint8_t> &data)
  {
    TransactionProto transaction_proto;
    transaction_proto.ParseFromArray(data.data(), data.size());

    std::string tx_id = transaction_proto.tx_id();

    std::vector<TransactionInput> inputs;
    for (const auto &input_proto : transaction_proto.inputs())
    {
      inputs.emplace_back(TransactionInput(input_proto.tx_id(), input_proto.vout(), input_proto.script_sig()));
    }

    std::vector<TransactionOutput> outputs;
    for (const auto &output_proto : transaction_proto.outputs())
    {
      outputs.emplace_back(TransactionOutput(output_proto.address(), output_proto.amount()));
    }

    return Transaction(tx_id, inputs, outputs);
  }

  std::vector<uint8_t> TransactionOutput::serialize(const TransactionOutput &output)
  {
    TransactionOutputProto output_proto;
    output_proto.set_address(output.script_pubkey_);
    output_proto.set_amount(output.amount_);

    std::string serialized_data;
    output_proto.SerializeToString(&serialized_data);

    return std::vector<uint8_t>(serialized_data.begin(), serialized_data.end());
  }

  TransactionOutput TransactionOutput::deserialize(const std::vector<uint8_t> &data)
  {
    TransactionOutputProto output_proto;
    output_proto.ParseFromArray(data.data(), data.size());

    return TransactionOutput(output_proto.address(), output_proto.amount());
  }

  std::vector<uint8_t> TransactionInput::serialize(const TransactionInput &input)
  {
    TransactionInputProto input_proto;
    input_proto.set_tx_id(input.tx_id_);
    input_proto.set_vout(input.vout_);
    input_proto.set_script_sig(input.script_sig_);

    std::string serialized_data;
    input_proto.SerializeToString(&serialized_data);

    return std::vector<uint8_t>(serialized_data.begin(), serialized_data.end());
  }

  TransactionInput TransactionInput::deserialize(const std::vector<uint8_t> &data)
  {
    TransactionInputProto input_proto;
    input_proto.ParseFromArray(data.data(), data.size());

    return TransactionInput(
        input_proto.tx_id(),
        input_proto.vout(),
        input_proto.script_sig());
  }

  std::string Transaction::toString() const
  {
    std::stringstream ss;
    ss << "Transaction ID: " << tx_id_.toHex() << std::endl;
    ss << "Inputs: " << std::endl;
    for (const auto &input : inputs_)
    {
      ss << "  " << input.toString() << std::endl;
    }
    ss << "Outputs: " << std::endl;
    for (const auto &output : outputs_)
    {
      ss << "  " << output.toString() << std::endl;
    }
    return ss.str();
  }

  std::string TransactionOutput::toString() const
  {
    return "Output: " + script_pubkey_ + " " + amount_;
  }

  std::string TransactionInput::toString() const
  {
    return "Input: " + tx_id_ + " " + std::to_string(vout_) + " " + script_sig_;
  }

}
