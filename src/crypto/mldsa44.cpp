// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/mldsa44.h>

namespace crypto {
namespace mldsa44 {

bool GenerateKeyPair(KeyPair& keypair)
{
    // Fail closed until a vetted FIPS 204 backend is integrated. Clear any
    // previous material so callers cannot mistake stale output for a new key.
    keypair.pubkey.data.clear();
    keypair.seckey.data.clear();
    return false;
}

bool Sign(const SecretKey& seckey, std::span<const unsigned char> message, std::span<const unsigned char> context, Signature& sig)
{
    // Fail closed until the backend, context rules, and serialization are defined.
    // Never leave a previous signature in the output on failure.
    (void)seckey;
    (void)message;
    (void)context;
    sig.data.clear();
    return false;
}

bool Verify(const PublicKey& pubkey, std::span<const unsigned char> message, std::span<const unsigned char> context, std::span<const unsigned char> sig)
{
    // Fail closed until a vetted verifier and canonical encodings are integrated.
    (void)pubkey;
    (void)message;
    (void)context;
    (void)sig;
    return false;
}

} // namespace mldsa44
} // namespace crypto
