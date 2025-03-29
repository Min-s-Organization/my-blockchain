#pragma once
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <string>
#include <vector>
#include <cstdint>
#include <openssl/ec.h>

namespace block_chain
{
  class Wallet
  {
  public:
    Wallet(EC_KEY *private_key, std::string hex_public_key) : private_key_(private_key), hex_public_key_(hex_public_key) {}
    ~Wallet() { EC_KEY_free(private_key_); }

    static Wallet createWallet();
    static Wallet createWallet(std::string private_key_in_hex);
    std::string publicKeyInHex() const { return hex_public_key_; }
    std::string privateKeyInHex() const { return privateKeyInHex(private_key_); }
    std::vector<uint8_t> sign(const std::string &message) const;

    static std::string privateKeyInHex(EC_KEY *private_key);
    static std::pair<EC_KEY *, std::string> generateKeypair(std::string &private_key_in_hex);

  private:
    EC_KEY *private_key_;
    std::string hex_public_key_;
  };
}