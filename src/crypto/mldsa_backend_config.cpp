// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/mldsa_backend_config.h>

namespace crypto {
namespace mldsa {

bool IsBackendConfigValid(const BackendConfig& config)
{
    if (config.strict_validation && config.allow_external_backend) {
        return false;
    }

    if (config.selection == BackendSelection::External) {
        return false;
    }

    return config.selection == BackendSelection::None ||
           config.selection == BackendSelection::Null;
}

std::unique_ptr<IMldsaBackend> ResolveConfiguredBackend(const BackendConfig& config)
{
    if (!IsBackendConfigValid(config)) {
        return nullptr;
    }

    return BackendSelector::CreateBackend(config.selection);
}

} // namespace mldsa
} // namespace crypto
