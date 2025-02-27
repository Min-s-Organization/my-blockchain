#include "Block.h"
#include <iostream>
#include <iomanip>

namespace block_chain
{
  void Block::setHash()
  {
    // create context
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr)
      exit(-1);

    // init
    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr))
    {
      EVP_MD_CTX_free(mdctx);
      exit(-1);
    }

    // add timestamp
    if (1 != EVP_DigestUpdate(mdctx, reinterpret_cast<const uint8_t *>(&timestamp_), sizeof(timestamp_)))
    {
      EVP_MD_CTX_free(mdctx);
      exit(-1);
    }

    // add data
    if (!data_.empty())
    {
      if (1 != EVP_DigestUpdate(mdctx, data_.data(), data_.size()))
      {
        EVP_MD_CTX_free(mdctx);
        exit(-1);
      }
    }

    // add prev hash
    if (!prev_hash_.empty())
    {
      if (1 != EVP_DigestUpdate(mdctx, prev_hash_.data(), prev_hash_.size()))
      {
        EVP_MD_CTX_free(mdctx);
        exit(-1);
      }
    }

    hash_.resize(EVP_MD_size(EVP_sha256()));
    // generate hash
    unsigned int md_len;
    if (1 != EVP_DigestFinal_ex(mdctx, hash_.data(), &md_len))
    {
      EVP_MD_CTX_free(mdctx);
      exit(-1);
    }

    EVP_MD_CTX_free(mdctx);
  }
}