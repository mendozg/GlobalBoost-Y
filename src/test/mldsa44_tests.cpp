// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/mldsa44.h>

#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

#include <array>

BOOST_FIXTURE_TEST_SUITE(mldsa44_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(mldsa44_scaffold_fails_closed)
{
    // The current implementation is only a scaffold. Until a vetted FIPS 204
    // backend is integrated, it must never report successful crypto operations.
    crypto::mldsa44::KeyPair keypair;
    BOOST_CHECK(!crypto::mldsa44::GenerateKeyPair(keypair));

    const std::array<unsigned char, 1> message{0x42};
    const std::array<unsigned char, 1> context{0x01};
    crypto::mldsa44::Signature signature;
    BOOST_CHECK(!crypto::mldsa44::Sign(keypair.seckey, message, context, signature));
    BOOST_CHECK(!crypto::mldsa44::Verify(keypair.pubkey, message, context, signature.data));
}

BOOST_AUTO_TEST_SUITE_END()
