#include "Wallet.h"
#include <iostream>
#include <cassert>

int main()
{
  std::cout << "Creating wallet..." << std::endl;

  // Test createWallet
  block_chain::Wallet wallet = block_chain::Wallet::createWallet();
  std::cout << "Private key: " << wallet.privateKeyInHex() << std::endl;
  std::cout << "Public key: " << wallet.publicKeyInHex() << std::endl;

  // Test generateKeypair
  std::string private_key_in_hex = wallet.privateKeyInHex();
  auto [private_key, public_key] = block_chain::Wallet::generateKeypair(private_key_in_hex);
  std::cout << "Private key: " << private_key_in_hex << std::endl;
  std::cout << "Public key: " << public_key << std::endl;
  auto wallet2 = block_chain::Wallet(private_key, public_key);
  assert(wallet2.privateKeyInHex() == wallet.privateKeyInHex());
  assert(wallet2.publicKeyInHex() == wallet.publicKeyInHex());

  std::cout << "Finished!" << std::endl;
}
