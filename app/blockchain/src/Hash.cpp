#include "Hash.h"
#include <openssl/evp.h>
#include <iostream>
#include <iomanip>

namespace block_chain
{
  Hash::Hash(const std::vector<const uint8_t *> &input)
  {
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr)
      exit(-1);

    // init
    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr))
    {
      EVP_MD_CTX_free(mdctx);
      exit(-1);
    }

    // add input to calculate hash
    for (const auto &data : input)
    {
      if (1 != EVP_DigestUpdate(mdctx, data, sizeof(data)))
      {
        EVP_MD_CTX_free(mdctx);
        exit(-1);
      }
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
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (const auto &byte : this->hash_)
    {
      ss << std::setw(2) << static_cast<int>(byte);
    }

    return ss.str();
  }
}
