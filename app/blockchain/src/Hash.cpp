#include "Hash.h"
#include <openssl/evp.h>
#include "Utils.h"
#include <iostream>

namespace block_chain
{
  Hash::Hash()
  {
    hash_ = std::vector<uint8_t>();
  }

  Hash::Hash(const std::vector<uint8_t> &input, bool is_hash)
  {
    if (is_hash)
    {
      hash_ = input;
      return;
    }

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr)
      exit(-1);

    // init
    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr))
    {
      EVP_MD_CTX_free(mdctx);
      exit(-1);
    }

    if (1 != EVP_DigestUpdate(mdctx, input.data(), input.size()))
    {
      EVP_MD_CTX_free(mdctx);
      exit(-1);
    }

    this->hash_.resize(EVP_MD_size(EVP_sha256()));

    // generate hash
    unsigned int md_len;
    if (1 != EVP_DigestFinal_ex(mdctx, hash_.data(), &md_len))
    {
      EVP_MD_CTX_free(mdctx);
      exit(-1);
    }

    EVP_MD_CTX_free(mdctx);
  }

  std::string Hash::toHex() const
  {
    return Utils::bytesToHex(this->hash_);
  }
}
