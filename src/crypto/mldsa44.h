// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GLOBALBOOST_CRYPTO_MLDSA44_H
#define GLOBALBOOST_CRYPTO_MLDSA44_H

#include <span>
#include <vector>

namespace crypto {
namespace mldsa44 {

struct PublicKey {
    std::vector<unsigned char> data;
};

struct SecretKey {
    std::vector<unsigned char> data;
};

struct Signature {
    std::vector<unsigned char> data;
};

struct KeyPair {
    PublicKey pubkey;
    SecretKey seckey;
};

bool GenerateKeyPair(KeyPair& keypair);
bool Sign(const SecretKey& seckey, std::span<const unsigned char> message, std::span<const unsigned char> context, Signature& sig);
bool Verify(const PublicKey& pubkey, std::span<const unsigned char> message, std::span<const unsigned char> context, std::span<const unsigned char> sig);

} // namespace mldsa44
} // namespace crypto

#endif // GLOBALBOOST_CRYPTO_MLDSA44_H
