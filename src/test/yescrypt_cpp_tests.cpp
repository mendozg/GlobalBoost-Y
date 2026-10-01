// Copyright (c) 2026 The GlobalBoost developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <crypto/yescrypt/yescrypt_cpp.h>

#include <array>
#include <cstdint>

#include <boost/test/unit_test.hpp>

extern "C" void yescrypt_hash(const char* input, char* output);

BOOST_AUTO_TEST_SUITE(yescrypt_cpp_tests)

BOOST_AUTO_TEST_CASE(constants)
{
    BOOST_CHECK_EQUAL(yescrypt::Hash::INPUT_SIZE, 80U);
    BOOST_CHECK_EQUAL(yescrypt::Hash::OUTPUT_SIZE, 32U);
}

BOOST_AUTO_TEST_CASE(matches_legacy_c_entry_point)
{
    using Input = std::array<std::uint8_t, yescrypt::Hash::INPUT_SIZE>;
    using Output = std::array<std::uint8_t, yescrypt::Hash::OUTPUT_SIZE>;

    const std::array<Input, 3> inputs{
        Input{},
        [] {
            Input input{};
            for (std::size_t i = 0; i < input.size(); ++i) {
                input[i] = static_cast<std::uint8_t>(i);
            }
            return input;
        }(),
        [] {
            Input input{};
            input.fill(0xa5);
            return input;
        }(),
    };

    for (const auto& input : inputs) {
        Output expected{};
        Output actual{};

        yescrypt_hash(reinterpret_cast<const char*>(input.data()),
                      reinterpret_cast<char*>(expected.data()));
        const auto result = yescrypt::Hash::Compute(input, actual);

        BOOST_CHECK(result == yescrypt::Error::SUCCESS);
        BOOST_CHECK(actual == expected);
    }
}

BOOST_AUTO_TEST_CASE(is_deterministic_and_overwrites_output)
{
    using Input = std::array<std::uint8_t, yescrypt::Hash::INPUT_SIZE>;
    using Output = std::array<std::uint8_t, yescrypt::Hash::OUTPUT_SIZE>;

    Input input{};
    input.fill(0x3c);

    Output first{};
    Output second{};
    BOOST_CHECK(yescrypt::Hash::Compute(input, first) == yescrypt::Error::SUCCESS);

    second.fill(0xff);
    BOOST_CHECK(yescrypt::Hash::Compute(input, second) == yescrypt::Error::SUCCESS);
    BOOST_CHECK(second == first);
}

BOOST_AUTO_TEST_SUITE_END()
