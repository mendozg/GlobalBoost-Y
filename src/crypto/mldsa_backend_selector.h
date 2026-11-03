// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GLOBALBOOST_CRYPTO_MLDSA_BACKEND_SELECTOR_H
#define GLOBALBOOST_CRYPTO_MLDSA_BACKEND_SELECTOR_H

#include <crypto/mldsa_backend.h>

#include <memory>
#include <string>

namespace crypto {
namespace mldsa {

enum class BackendSelection {
    None,
    Null,
    External,
};

class BackendSelector
{
public:
    static std::unique_ptr<IMldsaBackend> CreateBackend(BackendSelection selection);
    static bool IsAllowedSelection(BackendSelection selection);
    static std::string SelectionName(BackendSelection selection);
};

} // namespace mldsa
} // namespace crypto

#endif // GLOBALBOOST_CRYPTO_MLDSA_BACKEND_SELECTOR_H
