// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GLOBALBOOST_CRYPTO_MLDSA_BACKEND_H
#define GLOBALBOOST_CRYPTO_MLDSA_BACKEND_H

#include <cstddef>
#include <span>
#include <vector>

namespace crypto {
namespace mldsa {

enum class BackendStatus {
    Unavailable,
    Disabled,
    Initialized,
    Error,
};

struct BackendCapabilities {
    bool supports_keygen = false;
    bool supports_sign = false;
    bool supports_verify = false;
    bool supports_hardened_validation = false;
};

class IMldsaBackend
{
public:
    virtual ~IMldsaBackend() = default;

    virtual BackendStatus GetStatus() const = 0;
    virtual BackendCapabilities GetCapabilities() const = 0;

    virtual bool GenerateKeyPair(std::vector<unsigned char>& public_key,
                                std::vector<unsigned char>& secret_key) = 0;
    virtual bool Sign(std::span<const unsigned char> secret_key,
                     std::span<const unsigned char> message,
                     std::span<const unsigned char> context,
                     std::vector<unsigned char>& signature) = 0;
    virtual bool Verify(std::span<const unsigned char> public_key,
                       std::span<const unsigned char> message,
                       std::span<const unsigned char> context,
                       std::span<const unsigned char> signature) = 0;
};

class NullMldsaBackend final : public IMldsaBackend
{
public:
    BackendStatus GetStatus() const override { return BackendStatus::Disabled; }
    BackendCapabilities GetCapabilities() const override { return {}; }

    bool GenerateKeyPair(std::vector<unsigned char>& public_key,
                        std::vector<unsigned char>& secret_key) override
    {
        public_key.clear();
        secret_key.clear();
        return false;
    }

    bool Sign(std::span<const unsigned char> secret_key,
              std::span<const unsigned char> message,
              std::span<const unsigned char> context,
              std::vector<unsigned char>& signature) override
    {
        (void)secret_key;
        (void)message;
        (void)context;
        signature.clear();
        return false;
    }

    bool Verify(std::span<const unsigned char> public_key,
                std::span<const unsigned char> message,
                std::span<const unsigned char> context,
                std::span<const unsigned char> signature) override
    {
        (void)public_key;
        (void)message;
        (void)context;
        (void)signature;
        return false;
    }
};

} // namespace mldsa
} // namespace crypto

#endif // GLOBALBOOST_CRYPTO_MLDSA_BACKEND_H
