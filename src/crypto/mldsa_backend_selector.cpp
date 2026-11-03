// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/mldsa_backend_selector.h>

namespace crypto {
namespace mldsa {

std::unique_ptr<IMldsaBackend> BackendSelector::CreateBackend(BackendSelection selection)
{
    if (!IsAllowedSelection(selection)) {
        return nullptr;
    }

    return std::make_unique<NullMldsaBackend>();
}

bool BackendSelector::IsAllowedSelection(BackendSelection selection)
{
    return selection == BackendSelection::None || selection == BackendSelection::Null;
}

std::string BackendSelector::SelectionName(BackendSelection selection)
{
    switch (selection) {
    case BackendSelection::None:
        return "none";
    case BackendSelection::Null:
        return "null";
    case BackendSelection::External:
        return "external";
    default:
        return "unknown";
    }
}

} // namespace mldsa
} // namespace crypto
