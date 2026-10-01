// Copyright (c) 2026 The GlobalBoost developers
// Distributed under the MIT software license.

#include "yescrypt_cpp.h"

#include "yescrypt.h"

namespace yescrypt {

Error Hash::Compute(
    const std::array<std::uint8_t, INPUT_SIZE>& input,
    std::array<std::uint8_t, OUTPUT_SIZE>& output) noexcept
{
    yescrypt_hash(reinterpret_cast<const char*>(input.data()),
                  reinterpret_cast<char*>(output.data()));
    return Error::SUCCESS;
}

} // namespace yescrypt
