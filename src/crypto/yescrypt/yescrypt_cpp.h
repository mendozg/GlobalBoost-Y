// Copyright (c) 2026 The GlobalBoost developers
// Distributed under the MIT software license.

#ifndef GLOBALBOOST_CRYPTO_YESCRYPT_CPP_H
#define GLOBALBOOST_CRYPTO_YESCRYPT_CPP_H

#include <array>
#include <cstdint>

namespace yescrypt {

/** Result of the Phase 1 C++ adapter. */
enum class Error : int {
    SUCCESS = 0,
    INVALID_INPUT = -1,
    HASH_FAILED = -2,
};

/**
 * Thin C++ adapter around the consensus-critical legacy yescrypt entry point.
 *
 * Phase 1 deliberately does not reimplement yescrypt. Keeping this adapter
 * small makes it possible to compare the future C++ implementation against
 * the existing C implementation byte-for-byte.
 */
class Hash {
public:
    static constexpr std::size_t INPUT_SIZE = 80;
    static constexpr std::size_t OUTPUT_SIZE = 32;

    [[nodiscard]] static Error Compute(
        const std::array<std::uint8_t, INPUT_SIZE>& input,
        std::array<std::uint8_t, OUTPUT_SIZE>& output) noexcept;
};

} // namespace yescrypt

#endif // GLOBALBOOST_CRYPTO_YESCRYPT_CPP_H
