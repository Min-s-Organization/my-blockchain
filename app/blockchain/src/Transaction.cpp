#include "Transaction.h"
#include "block.pb.h"
#include "Utils.h"
#include <string>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/ec.h>

namespace block_chain
{
  Transaction Transaction::createCoinbaseTransaction(std::string to_address, std::string data)
  {
    return Transaction(
        {TransactionInput("", -1, "", "")},
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
      inputs.emplace_back(TransactionInput(input_proto.tx_id(), input_proto.vout(), input_proto.signature(), input_proto.public_key()));
    }

    std::vector<TransactionOutput> outputs;
    for (const auto &output_proto : transaction_proto.outputs())
    {
      outputs.emplace_back(TransactionOutput(output_proto.public_key(), output_proto.amount()));
    }

    return Transaction(tx_id, inputs, outputs);
  }

  std::vector<uint8_t> TransactionOutput::serialize(const TransactionOutput &output)
  {
    TransactionOutputProto output_proto;
    output_proto.set_public_key(output.public_key_);
    output_proto.set_amount(output.amount_);

    std::string serialized_data;
    output_proto.SerializeToString(&serialized_data);

    return std::vector<uint8_t>(serialized_data.begin(), serialized_data.end());
  }

  TransactionOutput TransactionOutput::deserialize(const std::vector<uint8_t> &data)
  {
    TransactionOutputProto output_proto;
    output_proto.ParseFromArray(data.data(), data.size());

    return TransactionOutput(output_proto.public_key(), output_proto.amount());
  }

  std::vector<uint8_t> TransactionInput::serialize(const TransactionInput &input)
  {
    TransactionInputProto input_proto;
    input_proto.set_tx_id(input.tx_id_);
    input_proto.set_vout(input.vout_);
    input_proto.set_signature(input.signature_);
    input_proto.set_public_key(input.public_key_);

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
        input_proto.public_key(),
        input_proto.signature());
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
    std::stringstream ss;
    ss << "Output: " << std::endl;
    ss << "    pubkey: " << public_key_ << std::endl;
    ss << "    amount: " << amount_;
    return ss.str();
  }

  std::string TransactionInput::toString() const
  {
    std::stringstream ss;
    ss << "Input: " << std::endl;
    ss << "    tx_id: " << tx_id_ << std::endl;
    ss << "    vout: " << std::to_string(vout_) << std::endl;
    ss << "    signature: " << signature_ << std::endl;
    ss << "    public_key: " << public_key_;
    return ss.str();
  }

  bool TransactionInput::verify(const std::string &hex_public_key)
  {
    std::vector<uint8_t> signature = Utils::hexToBytes(signature_);
    std::vector<uint8_t> message_bytes = Utils::hexToBytes(message());
    std::vector<uint8_t> public_key_bytes = Utils::hexToBytes(hex_public_key);

    EC_KEY *public_key = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!public_key)
    {
      return false;
    }
    const EC_GROUP *group = EC_KEY_get0_group(public_key);

    // get the pubkey point
    EC_POINT *pub_key = EC_POINT_new(group);
    if (!pub_key)
    {
      EC_KEY_free(public_key);
      return false;
    }

    // set the public key
    if (!EC_KEY_set_public_key(public_key, pub_key))
    {
      EC_POINT_free(pub_key);
      EC_KEY_free(public_key);
      return false;
    }

    // calculate the message hash
    std::vector<unsigned char> hash = Hash(message()).toBytes();

    // extract r and s values from the concatenated byte array
    int half_len = signature.size() / 2;

    // create objects to store r and s
    BIGNUM *r = BN_bin2bn(signature.data(), half_len, nullptr);
    BIGNUM *s = BN_bin2bn(signature.data() + half_len,
                          signature.size() - half_len, nullptr);

    if (!r || !s)
    {
      if (r)
        BN_free(r);
      if (s)
        BN_free(s);
      EC_POINT_free(pub_key);
      EC_KEY_free(public_key);
      return false;
    }

    // create the signature object
    ECDSA_SIG *sig = ECDSA_SIG_new();
    if (!sig)
    {
      BN_free(r);
      BN_free(s);
      EC_POINT_free(pub_key);
      EC_KEY_free(public_key);
      return false;
    }

    // set r and s values
    if (ECDSA_SIG_set0(sig, r, s) != 1)
    {
      BN_free(r);
      BN_free(s);
      ECDSA_SIG_free(sig);
      EC_POINT_free(pub_key);
      EC_KEY_free(public_key);
      return false;
    }

    int result = ECDSA_do_verify(hash.data(), hash.size(), sig, public_key);

    ECDSA_SIG_free(sig);
    EC_POINT_free(pub_key);
    EC_KEY_free(public_key);
    return result == 1;
  }
}