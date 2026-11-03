// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/mldsa_backend_config.h>

#include <boost/test/unit_test.hpp>

using namespace crypto::mldsa;

BOOST_AUTO_TEST_SUITE(mldsa_backend_config_tests)

BOOST_AUTO_TEST_CASE(config_accepts_only_null_or_none)
{
    BackendConfig cfg_none;
    cfg_none.selection = BackendSelection::None;
    BOOST_CHECK(IsBackendConfigValid(cfg_none));

    BackendConfig cfg_null;
    cfg_null.selection = BackendSelection::Null;
    BOOST_CHECK(IsBackendConfigValid(cfg_null));

    BackendConfig cfg_external;
    cfg_external.selection = BackendSelection::External;
    BOOST_CHECK(!IsBackendConfigValid(cfg_external));

    BackendConfig cfg_external_allowed;
    cfg_external_allowed.selection = BackendSelection::External;
    cfg_external_allowed.allow_external_backend = true;
    BOOST_CHECK(!IsBackendConfigValid(cfg_external_allowed));
}

BOOST_AUTO_TEST_CASE(config_rejects_external_backend_even_if_strict_validation_disabled)
{
    BackendConfig cfg;
    cfg.selection = BackendSelection::External;
    cfg.allow_external_backend = false;
    cfg.strict_validation = false;

    BOOST_CHECK(!IsBackendConfigValid(cfg));
}

BOOST_AUTO_TEST_CASE(resolve_config_returns_null_for_invalid_selection)
{
    BackendConfig invalid;
    invalid.selection = BackendSelection::External;

    auto backend = ResolveConfiguredBackend(invalid);
    BOOST_CHECK(!backend);
}

BOOST_AUTO_TEST_CASE(resolve_config_returns_null_backend_for_valid_selection)
{
    BackendConfig none;
    none.selection = BackendSelection::None;

    auto backend = ResolveConfiguredBackend(none);
    BOOST_REQUIRE(backend);
    BOOST_CHECK(backend->GetStatus() == BackendStatus::Disabled);

    BackendConfig null_cfg;
    null_cfg.selection = BackendSelection::Null;

    auto null_backend = ResolveConfiguredBackend(null_cfg);
    BOOST_REQUIRE(null_backend);
    BOOST_CHECK(null_backend->GetStatus() == BackendStatus::Disabled);
}

BOOST_AUTO_TEST_CASE(selector_rejects_external_backend)
{
    BOOST_CHECK(!BackendSelector::IsAllowedSelection(BackendSelection::External));
    BOOST_CHECK(!BackendSelector::CreateBackend(BackendSelection::External));
}

BOOST_AUTO_TEST_CASE(selector_accepts_null_and_none)
{
    auto none_backend = BackendSelector::CreateBackend(BackendSelection::None);
    BOOST_REQUIRE(none_backend);
    BOOST_CHECK(none_backend->GetStatus() == BackendStatus::Disabled);

    auto null_backend = BackendSelector::CreateBackend(BackendSelection::Null);
    BOOST_REQUIRE(null_backend);
    BOOST_CHECK(null_backend->GetStatus() == BackendStatus::Disabled);
}

BOOST_AUTO_TEST_SUITE_END()
