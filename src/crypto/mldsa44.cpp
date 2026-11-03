// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/mldsa44.h>

#include <span>
#include <vector>

namespace crypto {
namespace mldsa44 {

bool GenerateKeyPair(KeyPair& keypair)
{
    // This is a scaffold only.
    // Real ML-DSA-44 key generation must be implemented with a vetted FIPS 204 backend.
    // Do not return success without a real implementation.
    (void)keypair;
    return false;
}

bool Sign(const SecretKey& seckey, std::span<const unsigned char> message, std::span<const unsigned char> context, Signature& sig)
{
    // This is a scaffold only.
    // Real ML-DSA-44 signing must include exact hash/digest rules and context separation.
    (void)seckey;
    (void)message;
    (void)context;
    (void)sig;
    return false;
}

bool Verify(const PublicKey& pubkey, std::span<const unsigned char> message, std::span<const unsigned char> context, std::span<const unsigned char> sig)
{
    // This is a scaffold only.
    // Real verification must validate canonical serialization, lengths, and signature semantics.
    (void)pubkey;
    (void)message;
    (void)context;
    (void)sig;
    return false;
}

} // namespace mldsa44
} // namespace crypto
