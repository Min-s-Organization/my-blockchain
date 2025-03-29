#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <cassert>
#include <iostream>
#include "Hash.h"
#include "Utils.h"
#include "Wallet.h"

namespace block_chain
{
  Wallet Wallet::createWallet()
  {
    EC_KEY *private_key = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!private_key)
    {
      exit(-1);
    }

    // generate the key
    if (!EC_KEY_generate_key(private_key))
    {
      EC_KEY_free(private_key);
      exit(-1);
    }

    // change the private key to hex
    // const BIGNUM *priv_key = EC_KEY_get0_private_key(private_key);
    // std::string hex_priv_key = BN_bn2hex(priv_key);

    // get the public key
    const EC_POINT *pub_point = EC_KEY_get0_public_key(private_key);
    const EC_GROUP *group = EC_KEY_get0_group(private_key);

    // change the public key to hex
    std::string hex_pub_key = EC_POINT_point2hex(group, pub_point, POINT_CONVERSION_COMPRESSED, nullptr);

    return Wallet(private_key, hex_pub_key);
  }

  Wallet Wallet::createWallet(std::string private_key_in_hex)
  {
    std::pair<EC_KEY *, std::string> keypair = generateKeypair(private_key_in_hex);
    return Wallet(keypair.first, keypair.second);
  }

  std::vector<uint8_t> Wallet::sign(const std::string &message) const
  {
    // calculate the hash of the message
    std::vector<unsigned char> hash = Hash(message).toBytes();
    assert(private_key_ != nullptr);

    // sign the hash
    ECDSA_SIG *signature = ECDSA_do_sign(hash.data(), hash.size(), private_key_);
    if (!signature)
    {
      return {};
    }

    // get the r and s values
    const BIGNUM *r = ECDSA_SIG_get0_r(signature);
    const BIGNUM *s = ECDSA_SIG_get0_s(signature);

    // convert the r and s values to bytes
    std::vector<uint8_t> r_bytes(BN_num_bytes(r));
    BN_bn2bin(r, r_bytes.data());
    std::vector<uint8_t> s_bytes(BN_num_bytes(s));
    BN_bn2bin(s, s_bytes.data());

    // combine the r and s values into a single vector
    std::vector<uint8_t> signature_bytes(r_bytes.size() + s_bytes.size());
    std::copy(r_bytes.begin(), r_bytes.end(), signature_bytes.begin());
    std::copy(s_bytes.begin(), s_bytes.end(), signature_bytes.begin() + r_bytes.size());

    // free the signature
    ECDSA_SIG_free(signature);

    return signature_bytes;
  }

  std::string Wallet::privateKeyInHex(EC_KEY *private_key)
  {
    const BIGNUM *priv_key = EC_KEY_get0_private_key(private_key);
    return BN_bn2hex(priv_key);
  }

  std::pair<EC_KEY *, std::string> Wallet::generateKeypair(std::string &private_key_in_hex)
  {
    EC_KEY *private_key = EC_KEY_new_by_curve_name(NID_secp256k1);
    std::vector<unsigned char> private_key_hex_bytes = Utils::hexToBytes(private_key_in_hex);

    // set the private key
    BIGNUM *bn = BN_bin2bn(private_key_hex_bytes.data(), private_key_hex_bytes.size(), nullptr);
    if (!EC_KEY_set_private_key(private_key, bn))
    {
      EC_KEY_free(private_key);
      BN_free(bn);
      return {};
    }

    const EC_GROUP *group = EC_KEY_get0_group(private_key);
    EC_POINT *pub_key = EC_POINT_new(group);
    if (!pub_key)
    {
      BN_free(bn);
      EC_KEY_free(private_key);
      return {};
    }

    // calculate the public key
    if (!EC_POINT_mul(group, pub_key, bn, nullptr, nullptr, nullptr))
    {
      EC_POINT_free(pub_key);
      BN_free(bn);
      EC_KEY_free(private_key);
      return {};
    }

    // set the public key
    if (!EC_KEY_set_public_key(private_key, pub_key))
    {
      EC_POINT_free(pub_key);
      BN_free(bn);
      EC_KEY_free(private_key);
      return {};
    }

    // change the public key to hex
    std::string hex_pub_key = EC_POINT_point2hex(group, pub_key, POINT_CONVERSION_COMPRESSED, nullptr);

    // free resources
    EC_POINT_free(pub_key);
    BN_free(bn);

    return {private_key, hex_pub_key};
  }
}