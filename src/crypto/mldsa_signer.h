// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GLOBALBOOST_CRYPTO_MLDSA_SIGNER_H
#define GLOBALBOOST_CRYPTO_MLDSA_SIGNER_H

#include <crypto/mldsa44.h>

#include <span>

namespace crypto {
namespace mldsa44 {

class MldsaSigner
{
public:
    explicit MldsaSigner(const SecretKey& secret_key);

    bool Sign(std::span<const unsigned char> message,
              std::span<const unsigned char> context,
              Signature& sig) const;

    bool Verify(std::span<const unsigned char> message,
                std::span<const unsigned char> context,
                std::span<const unsigned char> sig) const;

    PublicKey PublicKey() const;

private:
    SecretKey m_secret_key;
};

bool SignMessage(const SecretKey& secret_key,
                 std::span<const unsigned char> message,
                 std::span<const unsigned char> context,
                 Signature& sig);

bool VerifyMessage(const PublicKey& public_key,
                   std::span<const unsigned char> message,
                   std::span<const unsigned char> context,
                   std::span<const unsigned char> sig);

} // namespace mldsa44
} // namespace crypto

#endif // GLOBALBOOST_CRYPTO_MLDSA_SIGNER_H
