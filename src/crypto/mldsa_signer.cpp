// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/mldsa_signer.h>

namespace crypto {
namespace mldsa44 {

MldsaSigner::MldsaSigner(const SecretKey& secret_key)
    : m_secret_key(secret_key)
{
}

bool MldsaSigner::Sign(std::span<const unsigned char> message,
                       std::span<const unsigned char> context,
                       Signature& sig) const
{
    // Fail closed until a vetted ML-DSA backend and signed-message rules are in
    // place. Never return a stale signature on failure.
    (void)message;
    (void)context;
    (void)m_secret_key;
    sig.data.clear();
    return false;
}

bool MldsaSigner::Verify(std::span<const unsigned char> message,
                         std::span<const unsigned char> context,
                         std::span<const unsigned char> sig) const
{
    // This scaffold does not implement actual ML-DSA verification.
    (void)message;
    (void)context;
    (void)sig;
    return false;
}

PublicKey MldsaSigner::PublicKey() const
{
    return PublicKey{};
}

bool SignMessage(const SecretKey& secret_key,
                 std::span<const unsigned char> message,
                 std::span<const unsigned char> context,
                 Signature& sig)
{
    MldsaSigner signer(secret_key);
    return signer.Sign(message, context, sig);
}

bool VerifyMessage(const PublicKey& public_key,
                   std::span<const unsigned char> message,
                   std::span<const unsigned char> context,
                   std::span<const unsigned char> sig)
{
    (void)public_key;
    (void)message;
    (void)context;
    (void)sig;
    return false;
}

} // namespace mldsa44
} // namespace crypto
