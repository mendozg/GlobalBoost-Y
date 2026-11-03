// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/mldsa44.h>

#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

#include <span>
#include <type_traits>

namespace {
using GenerateKeyPairSignature = bool (*)(crypto::mldsa44::KeyPair&);
using SignSignature = bool (*)(const crypto::mldsa44::SecretKey&,
                               std::span<const unsigned char>,
                               std::span<const unsigned char>,
                               crypto::mldsa44::Signature&);
using VerifySignature = bool (*)(const crypto::mldsa44::PublicKey&,
                                 std::span<const unsigned char>,
                                 std::span<const unsigned char>,
                                 std::span<const unsigned char>);

static_assert(std::is_same_v<decltype(&crypto::mldsa44::GenerateKeyPair), GenerateKeyPairSignature>);
static_assert(std::is_same_v<decltype(&crypto::mldsa44::Sign), SignSignature>);
static_assert(std::is_same_v<decltype(&crypto::mldsa44::Verify), VerifySignature>);
} // namespace

BOOST_FIXTURE_TEST_SUITE(mldsa44_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(mldsa44_api_types_are_available)
{
    // This test checks the declared API surface only; it deliberately does not
    // call the scaffold functions, whose implementation is not yet in the
    // crypto library build.
    BOOST_CHECK(sizeof(crypto::mldsa44::KeyPair) > 0);
}

BOOST_AUTO_TEST_SUITE_END()
