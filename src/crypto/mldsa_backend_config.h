// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GLOBALBOOST_CRYPTO_MLDSA_BACKEND_CONFIG_H
#define GLOBALBOOST_CRYPTO_MLDSA_BACKEND_CONFIG_H

#include <crypto/mldsa_backend_selector.h>

namespace crypto {
namespace mldsa {

struct BackendConfig
{
    BackendSelection selection = BackendSelection::None;
    bool allow_external_backend = false;
    bool strict_validation = true;
};

bool IsBackendConfigValid(const BackendConfig& config);
std::unique_ptr<IMldsaBackend> ResolveConfiguredBackend(const BackendConfig& config);

} // namespace mldsa
} // namespace crypto

#endif // GLOBALBOOST_CRYPTO_MLDSA_BACKEND_CONFIG_H
