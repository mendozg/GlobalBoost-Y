# Phase 2: Yescrypt Algorithm Migration - C to Pure C++

## Detailed Implementation Guide

This document provides a step-by-step guide to migrate the yescrypt algorithm implementation from C to pure C++.

---

## Table of Contents

1. [Architecture Overview](#architecture-overview)
2. [Module Decomposition](#module-decomposition)
3. [Detailed Implementation Steps](#detailed-implementation-steps)
4. [Code Examples](#code-examples)
5. [Testing Strategy](#testing-strategy)
6. [Performance Considerations](#performance-considerations)
7. [Risk Mitigation](#risk-mitigation)

---

## Architecture Overview

### Current C Implementation Structure

```
yescrypt-best_c.h          (Selects SIMD vs non-SIMD variant)
    ↓
├── yescrypt-simd_c.h      (SSE2-optimized, ~1100 lines)
├── yescrypt-opt_c.h       (Non-SIMD optimized, ~950 lines)
└── yescrypt-platform_c.h  (Memory management, ~350 lines)
        ↑
    Uses:
├── sha256_c.h             (SHA-256 implementation)
├── sysendian.h            (Endianness utilities)
└── yescrypt.h             (Public API)
```

**Key Components:**

| Component | Lines | Responsibility | Priority |
|-----------|-------|-----------------|----------|
| **Memory Layer** | ~150 | Allocate/free regions, manage state | HIGH |
| **Salsa20 Core** | ~250 | Cryptographic stream cipher | HIGH |
| **BlockMix** | ~200 | BlockMix_salsa8/pwxform operations | HIGH |
| **SMix** | ~400 | Main scrypt memory-hard function | MEDIUM |
| **KDF** | ~250 | Public interface, parameter handling | MEDIUM |
| **S-box Transform** | ~150 | PWXFORM optimization (conditional) | LOW |

---

## Module Decomposition

### Phase 2a: Memory Management Layer

**Migrate:** `yescrypt-platform_c.h::init_region()`, `alloc_region()`, `free_region()`

**New Structure:**

```cpp
// src/crypto/yescrypt/yescrypt_memory.hpp
namespace yescrypt {

class Region {
private:
    std::vector<uint8_t> m_buffer;
    uint8_t* m_aligned_ptr = nullptr;
    size_t m_aligned_size = 0;

    // Static allocation size alignment (cache-line friendly)
    static constexpr size_t ALIGNMENT = 64;

public:
    Region() = default;
    ~Region() { deallocate(); }

    // Delete copy, allow move
    Region(const Region&) = delete;
    Region& operator=(const Region&) = delete;
    Region(Region&&) noexcept;
    Region& operator=(Region&&) noexcept;

    // Allocate memory
    bool allocate(size_t size);
    void deallocate();

    // Accessors
    uint8_t* get_aligned() const { return m_aligned_ptr; }
    size_t aligned_size() const { return m_aligned_size; }
    bool is_valid() const { return m_aligned_ptr != nullptr; }

    // Operator for pointer-style access
    operator uint8_t*() { return m_aligned_ptr; }
    operator const uint8_t*() const { return m_aligned_ptr; }
};

} // namespace yescrypt
```

**Implementation (yescrypt_memory.cpp):**

```cpp
namespace yescrypt {

Region::Region(Region&& other) noexcept
    : m_buffer(std::move(other.m_buffer)),
      m_aligned_ptr(other.m_aligned_ptr),
      m_aligned_size(other.m_aligned_size)
{
    other.m_aligned_ptr = nullptr;
    other.m_aligned_size = 0;
}

Region& Region::operator=(Region&& other) noexcept {
    if (this != &other) {
        deallocate();
        m_buffer = std::move(other.m_buffer);
        m_aligned_ptr = other.m_aligned_ptr;
        m_aligned_size = other.m_aligned_size;
        other.m_aligned_ptr = nullptr;
        other.m_aligned_size = 0;
    }
    return *this;
}

bool Region::allocate(size_t size) {
    if (size == 0) return false;
    
    try {
        // Allocate extra for alignment
        size_t alloc_size = size + ALIGNMENT - 1;
        m_buffer.resize(alloc_size);
        
        // Align pointer
        uintptr_t addr = reinterpret_cast<uintptr_t>(m_buffer.data());
        uintptr_t aligned_addr = (addr + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
        m_aligned_ptr = reinterpret_cast<uint8_t*>(aligned_addr);
        m_aligned_size = size;
        
        return true;
    } catch (const std::bad_alloc&) {
        deallocate();
        return false;
    }
}

void Region::deallocate() {
    m_buffer.clear();
    m_aligned_ptr = nullptr;
    m_aligned_size = 0;
}

} // namespace yescrypt
```

**Key Points:**
- ✅ RAII cleanup via destructor
- ✅ Move semantics for efficient transfer
- ✅ Cache-line alignment for performance
- ✅ Exception safety (catch std::bad_alloc)

---

### Phase 2b: Endianness & Utility Layer

**Migrate:** `sysendian.h`, `sha256_c.h` interfaces

**New Structure:**

```cpp
// src/crypto/yescrypt/yescrypt_endian.hpp
namespace yescrypt {

// Compile-time endianness detection
constexpr bool is_big_endian() {
    return std::endian::native == std::endian::big;
}

// Host → Little-endian conversions (64-bit)
inline uint64_t be64dec(const uint8_t* p) {
    return ((uint64_t)p[0] << 56) | ((uint64_t)p[1] << 48) |
           ((uint64_t)p[2] << 40) | ((uint64_t)p[3] << 32) |
           ((uint64_t)p[4] << 24) | ((uint64_t)p[5] << 16) |
           ((uint64_t)p[6] << 8)  | ((uint64_t)p[7]);
}

inline uint64_t le64dec(const uint8_t* p) {
    return ((uint64_t)p[7] << 56) | ((uint64_t)p[6] << 48) |
           ((uint64_t)p[5] << 40) | ((uint64_t)p[4] << 32) |
           ((uint64_t)p[3] << 24) | ((uint64_t)p[2] << 16) |
           ((uint64_t)p[1] << 8)  | ((uint64_t)p[0]);
}

inline void be64enc(uint8_t* p, uint64_t x) {
    p[0] = x >> 56; p[1] = x >> 48; p[2] = x >> 40; p[3] = x >> 32;
    p[4] = x >> 24; p[5] = x >> 16; p[6] = x >> 8;  p[7] = x;
}

inline void le64enc(uint8_t* p, uint64_t x) {
    p[7] = x >> 56; p[6] = x >> 48; p[5] = x >> 40; p[4] = x >> 32;
    p[3] = x >> 24; p[2] = x >> 16; p[1] = x >> 8;  p[0] = x;
}

// 32-bit variants (same pattern)
inline uint32_t be32dec(const uint8_t* p) { ... }
inline uint32_t le32dec(const uint8_t* p) { ... }
inline void be32enc(uint8_t* p, uint32_t x) { ... }
inline void le32enc(uint8_t* p, uint32_t x) { ... }

} // namespace yescrypt
```

**Rationale:**
- Replace C macro conditionals with `constexpr`
- Use `std::endian` (C++20) or fallback
- Inline for performance (compiler will optimize)

---

### Phase 2c: Cryptographic Primitives (Salsa20)

**Migrate:** `yescrypt-opt_c.h::salsa20_8()`, `blockmix_salsa8()`

**New Structure:**

```cpp
// src/crypto/yescrypt/yescrypt_salsa20.hpp
namespace yescrypt {

class Salsa20 {
public:
    // Block operations on 64-bit word arrays
    static void apply_core(std::array<uint64_t, 8>& block);
    
    // BlockMix operation
    static void blockmix(
        gsl::span<const uint64_t> input,
        gsl::span<uint64_t> output,
        std::array<uint64_t, 8>& temp,
        size_t r
    );
    
private:
    // Rotate-left utility
    static constexpr uint32_t rotl32(uint32_t x, unsigned n) {
        return (x << n) | (x >> (32 - n));
    }
};

} // namespace yescrypt
```

**Implementation Fragment:**

```cpp
void Salsa20::apply_core(std::array<uint64_t, 8>& block) {
    // Unshuffle: convert SIMD layout to scalar operations
    std::array<uint32_t, 16> x;
    unshuffle(block, x);
    
    // Apply Salsa20/8 rounds (4x double-round per cycle)
    for (int i = 0; i < 4; ++i) {
        // Column operations
        x[4]  ^= rotl32(x[0]  + x[12], 7);
        x[8]  ^= rotl32(x[4]  + x[0],  9);
        x[12] ^= rotl32(x[8]  + x[4],  13);
        x[0]  ^= rotl32(x[12] + x[8],  18);
        
        // ... (more column and row operations per Salsa20 spec)
        
        // Row operations
        x[1]  ^= rotl32(x[0]  + x[3],  7);
        x[2]  ^= rotl32(x[1]  + x[0],  9);
        x[3]  ^= rotl32(x[2]  + x[1],  13);
        x[0]  ^= rotl32(x[3]  + x[2],  18);
        
        // ... (continue for all 16 operations)
    }
    
    // Shuffle back & add
    shuffle_and_add(x, block);
}
```

**Key Improvements:**
- Use `std::array` for fixed-size buffers → bounds checking
- Use `gsl::span` for dynamic slices → no raw pointers
- `constexpr` rotl32 → compile-time optimization
- Clearer round unrolling

---

### Phase 2d: Block Transform (PWXForm)

**Migrate:** `yescrypt-opt_c.h::block_pwxform()`, `blockmix_pwxform()`

**New Structure:**

```cpp
// src/crypto/yescrypt/yescrypt_pwxform.hpp
namespace yescrypt {

class PWXForm {
public:
    // S-box configuration (const at compile time)
    struct Config {
        static constexpr int BITS = 8;      // S_BITS
        static constexpr int SIMD = 2;      // S_SIMD
        static constexpr int P = 4;         // S_P
        static constexpr int ROUNDS = 6;    // S_ROUNDS
        
        static constexpr size_t SIZE1 = 1 << BITS;
        static constexpr size_t MASK = (SIZE1 - 1) * SIMD * 8;
        static constexpr size_t SIZE_ALL = 2 * SIZE1 * SIMD; // S_N=2
    };
    
    // Apply transformation to block
    static void transform_block(
        gsl::span<uint64_t> block,
        gsl::span<const uint64_t> sboxes
    );
    
    // BlockMix with PWXForm
    static void blockmix(
        gsl::span<const uint64_t> input,
        gsl::span<uint64_t> output,
        gsl::span<const uint64_t> sboxes,
        size_t r
    );
};

} // namespace yescrypt
```

**Benefits:**
- `struct Config` replaces #define macros
- Bounds-safe via `gsl::span`
- Compile-time constants for optimization

---

### Phase 2e: Core SMix (Memory-Hard Function)

**Migrate:** `yescrypt-opt_c.h::smix1()`, `smix2()`, `smix()`

**New Structure:**

```cpp
// src/crypto/yescrypt/yescrypt_smix.hpp
namespace yescrypt {

class SMix {
public:
    enum class Flags : uint32_t {
        WORM = 0,
        RW = 1,
        PARALLEL_SMIX = 2,
        PWXFORM = 4
    };
    
    struct SharedState {
        Region rom;        // Read-only memory (initialized once)
        uint32_t mask = 1; // ROM access frequency mask
    };
    
    struct LocalState {
        Region ram;        // Thread-local RAM
    };
    
    // Main SMix computation
    static int compute(
        gsl::span<uint64_t> B,          // Input/output block
        size_t r,                        // Block size parameter
        uint64_t N,                      // Memory-hard parameter (power of 2)
        uint32_t p,                      // Parallelism parameter
        uint32_t t,                      // Time cost parameter
        Flags flags,
        const SharedState& shared,
        LocalState& local,
        gsl::span<uint8_t> output       // Final output
    );
    
private:
    // First mixing loop (initialization)
    static void smix1(
        gsl::span<uint64_t> B,
        size_t r,
        uint64_t N,
        Flags flags,
        gsl::span<uint64_t> V,      // Temporary memory
        uint64_t NROM,
        const SharedState& shared,
        gsl::span<uint64_t> XY,     // Working space
        gsl::span<uint64_t> S       // S-boxes (optional)
    );
    
    // Second mixing loop (scattering)
    static void smix2(
        gsl::span<uint64_t> B,
        size_t r,
        uint64_t N,
        uint64_t Nloop,
        Flags flags,
        gsl::span<uint64_t> V,
        uint64_t NROM,
        const SharedState& shared,
        gsl::span<uint64_t> XY,
        gsl::span<uint64_t> S
    );
    
    // Top-level orchestration
    static void smix_orchestrate(
        gsl::span<uint64_t> B,
        size_t r,
        uint64_t N,
        uint32_t p,
        uint32_t t,
        Flags flags,
        gsl::span<uint64_t> V,
        uint64_t NROM,
        const SharedState& shared,
        gsl::span<uint64_t> XY,
        gsl::span<uint64_t> S
    );
    
    // Utility: Largest power of 2 <= x
    static uint64_t p2floor(uint64_t x);
    
    // Utility: Extract integer from block (for random access)
    static uint64_t integerify(gsl::span<const uint64_t> B, size_t r);
};

} // namespace yescrypt
```

---

### Phase 2f: Public KDF Interface

**Migrate:** `yescrypt.c::yescrypt_hash()`, `yescrypt_bsty()`

**New Structure:**

```cpp
// src/crypto/yescrypt/yescrypt.hpp
namespace yescrypt {

class KDF {
public:
    // High-level hash function (matching legacy interface)
    static int hash(
        gsl::span<const uint8_t> input,
        gsl::span<uint8_t> output
    );
    
    // Full KDF with configuration
    struct Config {
        uint64_t N = 2048;          // Memory cost
        uint32_t r = 8;             // Block size
        uint32_t p = 1;             // Parallelism
        uint32_t t = 0;             // Time cost
        SMix::Flags flags = SMix::Flags::RW | SMix::Flags::PWXFORM;
    };
    
    static int derive(
        gsl::span<const uint8_t> password,
        gsl::span<const uint8_t> salt,
        const Config& config,
        gsl::span<uint8_t> output,
        SMix::SharedState& shared,
        SMix::LocalState& local
    );
    
    // Parameter validation
    static bool validate_params(const Config& config);
    
private:
    // Pre-hash password (for backwards compatibility)
    static void prehash_password(
        gsl::span<const uint8_t> password,
        std::array<uint64_t, 4>& sha256_out
    );
};

} // namespace yescrypt
```

**Implementation:**

```cpp
int KDF::hash(gsl::span<const uint8_t> input, gsl::span<uint8_t> output) {
    if (input.size() != 80 || output.size() != 32) {
        return -1; // EINVAL
    }
    
    // Thread-local static initialization (replace __thread with thread_local)
    thread_local static bool initialized = false;
    thread_local static SMix::SharedState shared;
    thread_local static SMix::LocalState local;
    
    if (!initialized) {
        // Dummy initialization (no ROM)
        // shared and local are default-constructed
        initialized = true;
    }
    
    Config cfg;
    return derive(input, input, cfg, output, shared, local);
}
```

---

## Detailed Implementation Steps

### Step 1: Create Directory Structure

```bash
mkdir -p src/crypto/yescrypt/cpp
cd src/crypto/yescrypt/cpp

# Create headers
touch yescrypt_common.hpp     # Enums, common types
touch yescrypt_memory.hpp      # Region RAII class
touch yescrypt_endian.hpp      # Endianness utilities
touch yescrypt_salsa20.hpp     # Salsa20 cipher
touch yescrypt_pwxform.hpp     # PWXForm S-box transform
touch yescrypt_smix.hpp        # SMix algorithm
touch yescrypt.hpp             # Public C++ API

# Create implementations
touch yescrypt_memory.cpp
touch yescrypt_salsa20.cpp
touch yescrypt_pwxform.cpp
touch yescrypt_smix.cpp
touch yescrypt.cpp
```

### Step 2: Implement Layer by Layer

**Order of Implementation (dependency-driven):**

1. **Common Types** (`yescrypt_common.hpp`)
   - Enums (Flags, error codes)
   - Type aliases
   - Config structs

2. **Memory Management** (`yescrypt_memory.*`)
   - `Region` class
   - Allocation/deallocation
   - Move semantics

3. **Utilities** (`yescrypt_endian.hpp`)
   - Endianness conversion functions
   - Compile-time helpers

4. **Salsa20** (`yescrypt_salsa20.*`)
   - Core round function
   - Block shuffling
   - BlockMix operation

5. **PWXForm** (`yescrypt_pwxform.*`)
   - S-box handling
   - Transform operations

6. **SMix** (`yescrypt_smix.*`)
   - smix1() loop
   - smix2() loop
   - Orchestration

7. **KDF** (`yescrypt.*`)
   - High-level interface
   - Parameter validation
   - Integration with SMix

### Step 3: Build System Integration

**Update `Makefile.am`:**

```makefile
# Add C++ crypto sources
noinst_HEADERS += \
    crypto/yescrypt/cpp/yescrypt.hpp \
    crypto/yescrypt/cpp/yescrypt_common.hpp \
    crypto/yescrypt/cpp/yescrypt_memory.hpp \
    crypto/yescrypt/cpp/yescrypt_endian.hpp \
    crypto/yescrypt/cpp/yescrypt_salsa20.hpp \
    crypto/yescrypt/cpp/yescrypt_pwxform.hpp \
    crypto/yescrypt/cpp/yescrypt_smix.hpp

libglobalboost_server_a_SOURCES += \
    crypto/yescrypt/cpp/yescrypt_memory.cpp \
    crypto/yescrypt/cpp/yescrypt_salsa20.cpp \
    crypto/yescrypt/cpp/yescrypt_pwxform.cpp \
    crypto/yescrypt/cpp/yescrypt_smix.cpp \
    crypto/yescrypt/cpp/yescrypt.cpp

# Set C++ standard (C++17 minimum)
if GCC
AM_CXXFLAGS += -std=c++17 -O3
endif
```

---

## Code Examples

### Example 1: Memory Layer Implementation

**File: `yescrypt_memory.hpp`**

```cpp
#pragma once

#include <cstdint>
#include <cstring>
#include <vector>
#include <memory>

namespace yescrypt {

/**
 * Memory region with automatic alignment and cleanup.
 * Uses RAII for deterministic resource management.
 */
class Region {
private:
    static constexpr size_t ALIGNMENT = 64; // Cache-line friendly

    std::vector<uint8_t> m_buffer;
    uint8_t* m_aligned = nullptr;
    size_t m_aligned_size = 0;

    void deallocate();

public:
    Region() = default;
    ~Region() { deallocate(); }

    // Prevent copying (deep copy would be expensive)
    Region(const Region&) = delete;
    Region& operator=(const Region&) = delete;

    // Allow move semantics
    Region(Region&& other) noexcept;
    Region& operator=(Region&& other) noexcept;

    /**
     * Allocate and align memory.
     * @param size Number of bytes to allocate
     * @return true on success, false on failure
     */
    bool allocate(size_t size);

    // Accessors
    uint8_t* data() { return m_aligned; }
    const uint8_t* data() const { return m_aligned; }
    size_t size() const { return m_aligned_size; }
    bool is_valid() const { return m_aligned != nullptr; }

    // Cast operators for compatibility
    operator uint8_t*() { return m_aligned; }
    operator const uint8_t*() const { return m_aligned; }
};

} // namespace yescrypt
```

**File: `yescrypt_memory.cpp`**

```cpp
#include "yescrypt_memory.hpp"
#include <algorithm>
#include <stdexcept>

namespace yescrypt {

Region::Region(Region&& other) noexcept
    : m_buffer(std::move(other.m_buffer)),
      m_aligned(other.m_aligned),
      m_aligned_size(other.m_aligned_size)
{
    other.m_aligned = nullptr;
    other.m_aligned_size = 0;
}

Region& Region::operator=(Region&& other) noexcept {
    if (this != &other) {
        deallocate();
        m_buffer = std::move(other.m_buffer);
        m_aligned = other.m_aligned;
        m_aligned_size = other.m_aligned_size;
        other.m_aligned = nullptr;
        other.m_aligned_size = 0;
    }
    return *this;
}

bool Region::allocate(size_t size) {
    if (size == 0) {
        return false;
    }

    try {
        // Allocate with extra space for alignment
        size_t alloc_size = size + ALIGNMENT - 1;
        m_buffer.resize(alloc_size);

        // Align pointer to ALIGNMENT boundary
        uintptr_t addr = reinterpret_cast<uintptr_t>(m_buffer.data());
        uintptr_t aligned_addr = (addr + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
        m_aligned = reinterpret_cast<uint8_t*>(aligned_addr);
        m_aligned_size = size;

        return true;
    } catch (const std::bad_alloc&) {
        deallocate();
        return false;
    }
}

void Region::deallocate() {
    m_buffer.clear();
    m_buffer.shrink_to_fit(); // Optional: free underlying memory
    m_aligned = nullptr;
    m_aligned_size = 0;
}

} // namespace yescrypt
```

### Example 2: Salsa20 Core

**File: `yescrypt_salsa20.hpp`**

```cpp
#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <gsl/span>

namespace yescrypt {

/**
 * Salsa20/8 stream cipher operations.
 */
class Salsa20 {
public:
    /**
     * Apply Salsa20/8 core to a block.
     * @param block 8x uint64_t block (modified in-place)
     */
    static void apply_core(std::array<uint64_t, 8>& block);

    /**
     * BlockMix operation: Salsa20/8 applied to interleaved blocks.
     * @param input Input block (2r * 8 uint64_t words)
     * @param output Output block (same size)
     * @param temp Temporary 64-byte workspace
     * @param r Block count parameter
     */
    static void blockmix(
        gsl::span<const uint64_t> input,
        gsl::span<uint64_t> output,
        std::array<uint64_t, 8>& temp,
        size_t r
    );

private:
    // Rotate left: (x << n) | (x >> (32 - n))
    static constexpr uint32_t rotl32(uint32_t x, unsigned n) {
        return (x << n) | (x >> (32 - n));
    }

    // Shuffle SIMD layout to scalar (128-bit block → 16x uint32)
    static void unshuffle(
        const std::array<uint64_t, 8>& in,
        std::array<uint32_t, 16>& out
    );

    // Shuffle scalar back to SIMD layout and accumulate
    static void shuffle_and_add(
        const std::array<uint32_t, 16>& in,
        std::array<uint64_t, 8>& out
    );
};

} // namespace yescrypt
```

**File: `yescrypt_salsa20.cpp` (Partial)**

```cpp
#include "yescrypt_salsa20.hpp"
#include "yescrypt_endian.hpp"

namespace yescrypt {

void Salsa20::apply_core(std::array<uint64_t, 8>& block) {
    std::array<uint32_t, 16> x;
    unshuffle(block, x);

    // Apply 8 rounds (4 double-rounds)
    for (int i = 0; i < 4; ++i) {
        // Operate on columns
        x[4] ^= rotl32(x[0] + x[12], 7);
        x[8] ^= rotl32(x[4] + x[0], 9);
        x[12] ^= rotl32(x[8] + x[4], 13);
        x[0] ^= rotl32(x[12] + x[8], 18);

        x[9] ^= rotl32(x[5] + x[1], 7);
        x[13] ^= rotl32(x[9] + x[5], 9);
        x[1] ^= rotl32(x[13] + x[9], 13);
        x[5] ^= rotl32(x[1] + x[13], 18);

        x[14] ^= rotl32(x[10] + x[6], 7);
        x[2] ^= rotl32(x[14] + x[10], 9);
        x[6] ^= rotl32(x[2] + x[14], 13);
        x[10] ^= rotl32(x[6] + x[2], 18);

        x[3] ^= rotl32(x[15] + x[11], 7);
        x[7] ^= rotl32(x[3] + x[15], 9);
        x[11] ^= rotl32(x[7] + x[3], 13);
        x[15] ^= rotl32(x[11] + x[7], 18);

        // Operate on rows
        x[1] ^= rotl32(x[0] + x[3], 7);
        x[2] ^= rotl32(x[1] + x[0], 9);
        x[3] ^= rotl32(x[2] + x[1], 13);
        x[0] ^= rotl32(x[3] + x[2], 18);

        x[6] ^= rotl32(x[5] + x[4], 7);
        x[7] ^= rotl32(x[6] + x[5], 9);
        x[4] ^= rotl32(x[7] + x[6], 13);
        x[5] ^= rotl32(x[4] + x[7], 18);

        x[11] ^= rotl32(x[10] + x[9], 7);
        x[8] ^= rotl32(x[11] + x[10], 9);
        x[9] ^= rotl32(x[8] + x[11], 13);
        x[10] ^= rotl32(x[9] + x[8], 18);

        x[12] ^= rotl32(x[15] + x[14], 7);
        x[13] ^= rotl32(x[12] + x[15], 9);
        x[14] ^= rotl32(x[13] + x[12], 13);
        x[15] ^= rotl32(x[14] + x[13], 18);
    }

    shuffle_and_add(x, block);
}

void Salsa20::blockmix(
    gsl::span<const uint64_t> input,
    gsl::span<uint64_t> output,
    std::array<uint64_t, 8>& temp,
    size_t r)
{
    // Copy B_{2r-1} to temp
    for (size_t i = 0; i < 8; ++i) {
        temp[i] = input[(2 * r - 1) * 8 + i];
    }

    // BlockMix loop: process each of 2r blocks
    for (size_t i = 0; i < 2 * r; ++i) {
        // temp ^= B_i
        for (size_t j = 0; j < 8; ++j) {
            temp[j] ^= input[i * 8 + j];
        }

        // temp = H(temp)
        apply_core(temp);

        // Y_i = temp (interleaved into output)
        size_t out_idx = (i < r) ? (i * 8) : ((i - r) * 8 + r * 8);
        for (size_t j = 0; j < 8; ++j) {
            output[out_idx + j] = temp[j];
        }
    }
}

} // namespace yescrypt
```

### Example 3: SMix Integration

**File: `yescrypt.hpp` (Public Interface)**

```cpp
#pragma once

#include <cstdint>
#include <array>
#include <gsl/span>
#include "yescrypt_smix.hpp"

namespace yescrypt {

/**
 * Primary C++ interface for yescrypt KDF.
 */
class KDF {
public:
    /**
     * Legacy yescrypt_hash: 80-byte input → 32-byte output.
     * Uses hardcoded parameters: N=2048, r=8, p=1, t=0, flags=RW|PWXFORM
     */
    static int hash(
        gsl::span<const uint8_t> input,  // 80 bytes
        gsl::span<uint8_t> output         // 32 bytes
    );

    /**
     * Full KDF with custom configuration.
     */
    struct Config {
        uint64_t N = 2048;
        uint32_t r = 8;
        uint32_t p = 1;
        uint32_t t = 0;
        SMix::Flags flags = SMix::Flags::RW | SMix::Flags::PWXFORM;
    };

    static int derive(
        gsl::span<const uint8_t> password,
        gsl::span<const uint8_t> salt,
        const Config& config,
        gsl::span<uint8_t> output,
        SMix::SharedState& shared,
        SMix::LocalState& local
    );

    /**
     * Validate KDF parameters.
     */
    static bool validate_params(const Config& config);

private:
    // Pre-hash password for security
    static void prehash_password(
        gsl::span<const uint8_t> password,
        std::array<uint64_t, 4>& sha256_out
    );
};

} // namespace yescrypt
```

---

## Testing Strategy

### Unit Tests Template

**File: `src/test/crypto/yescrypt_tests.cpp`**

```cpp
#include <boost/test/unit_test.hpp>
#include "crypto/yescrypt/cpp/yescrypt.hpp"
#include <vector>
#include <array>

using namespace yescrypt;

BOOST_AUTO_TEST_SUITE(YescryptTests)

// Test 1: Known test vectors
BOOST_AUTO_TEST_CASE(test_hash_vectors) {
    // Input: 80 bytes of 0x00
    std::vector<uint8_t> input(80, 0x00);
    std::array<uint8_t, 32> output;
    
    int result = KDF::hash(input, output);
    BOOST_REQUIRE_EQUAL(result, 0);
    
    // Expected output (from original C implementation or reference)
    const std::array<uint8_t, 32> expected = { /* ... */ };
    BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(),
                                   expected.begin(), expected.end());
}

// Test 2: Memory alignment
BOOST_AUTO_TEST_CASE(test_memory_alignment) {
    Region r;
    BOOST_REQUIRE(r.allocate(1024));
    BOOST_CHECK_EQUAL(reinterpret_cast<uintptr_t>(r.data()) % 64, 0);
}

// Test 3: Move semantics
BOOST_AUTO_TEST_CASE(test_region_move) {
    Region r1, r2;
    r1.allocate(512);
    uint8_t* ptr1 = r1.data();
    
    r2 = std::move(r1);
    BOOST_CHECK_EQUAL(r2.data(), ptr1);
    BOOST_CHECK(r1.data() == nullptr);
}

// Test 4: Parameter validation
BOOST_AUTO_TEST_CASE(test_invalid_params) {
    KDF::Config cfg;
    cfg.N = 3; // Not a power of 2
    BOOST_CHECK(!KDF::validate_params(cfg));
}

BOOST_AUTO_TEST_SUITE_END()
```

### Regression Tests

**Create `test_yescrypt_compat.py`:**

```python
#!/usr/bin/env python3
"""
Compare new C++ implementation against original C implementation.
"""
import subprocess
import sys

def hash_with_cpp(data):
    # Call C++ binary or library function
    pass

def hash_with_c(data):
    # Call original C implementation
    pass

def test_vectors():
    test_cases = [
        bytes(80),  # All zeros
        bytes(range(256)) * 1,  # Sequential bytes
        b"GlobalBoost" + bytes(69),  # Known prefix
    ]
    
    for i, tc in enumerate(test_cases):
        cpp_out = hash_with_cpp(tc)
        c_out = hash_with_c(tc)
        assert cpp_out == c_out, f"Mismatch in test case {i}"
        print(f"✓ Test case {i} passed")

if __name__ == "__main__":
    test_vectors()
    print("All tests passed!")
```

---

## Performance Considerations

### Optimization Checklist

| Area | Strategy |
|------|----------|
| **Memory Layout** | Align to cache lines (64B), minimize false sharing |
| **SIMD** | Conditional: compile with `-msse2` / `-mavx` flags |
| **Loop Unrolling** | Compiler handles via `-O3`; explicit unroll for crypto |
| **Parallelism** | OpenMP `#pragma omp parallel for` (via existing code) |
| **Branch Prediction** | Minimize conditional branches in inner loops |
| **Inlining** | Use `inline` / `constexpr` for small utility functions |

### Benchmark Template

**File: `src/test/crypto/yescrypt_benchmark.cpp`**

```cpp
#include <benchmark/benchmark.h>
#include "crypto/yescrypt/cpp/yescrypt.hpp"

static void BM_Yescrypt_Hash(benchmark::State& state) {
    std::vector<uint8_t> input(80);
    std::array<uint8_t, 32> output;
    
    for (auto _ : state) {
        yescrypt::KDF::hash(input, output);
    }
}

BENCHMARK(BM_Yescrypt_Hash);
BENCHMARK_MAIN();
```

Run:
```bash
./yescrypt_benchmark --benchmark_repetitions=5
```

---

## Risk Mitigation

### 1. **Correctness Risk**

| Risk | Mitigation |
|------|-----------|
| Algorithm deviation | Maintain byte-for-byte test vectors against C version |
| Memory corruption | Use `gsl::span` (bounds checking), ASAN in CI |
| Uninitialized values | Aggregate-initialize all members (`Region{}`), use `= default` |

### 2. **Performance Risk**

| Risk | Mitigation |
|------|-----------|
| Slowdown from C++ overhead | Profile with `perf`, use inline / constexpr liberally |
| Memory bloat | Monitor binary size, optimize hot paths |
| Cache misses | Align allocations, minimize pointer chasing |

### 3. **Integration Risk**

| Risk | Mitigation |
|------|-----------|
| Legacy C interface breakage | Keep wrapper in `src/hash.h`, gradual migration |
| Thread-local issues | Use C++11 `thread_local` (replace `__thread`) |
| Build system failure | Test with multiple compilers: g++11, clang++14 |

### 4. **Testing Strategy**

```bash
# Phase 2 Validation Checklist

# 1. Unit tests
make check

# 2. Regression: C vs C++
test/functional/test_yescrypt_compat.py

# 3. Blockchain consensus
test/functional/test_blockchain.py

# 4. Performance regression
./src/test/crypto/yescrypt_benchmark

# 5. Memory safety
ASAN_OPTIONS=detect_leaks=1 make check

# 6. Thread safety (if using thread_local)
stress_test/yescrypt_multithreaded
```

---

## Compilation Flags

### Recommended Build Configuration

```bash
./configure CXXFLAGS="-std=c++17 -O3 -march=native" \
            CFLAGS="-O2" \
            --enable-yescrypt-cpp \
            --with-gsl-include=/usr/include

make -j$(nproc)
make check
```

### Feature Flags

Add to `configure.ac`:

```m4
AC_ARG_ENABLE([yescrypt-cpp],
    [AS_HELP_STRING([--enable-yescrypt-cpp],
        [Enable C++ yescrypt implementation (default: yes)])],
    [ac_yescrypt_cpp=$enableval],
    [ac_yescrypt_cpp=yes])

if test "$ac_yescrypt_cpp" = "yes"; then
    AC_DEFINE([HAVE_YESCRYPT_CPP], [1], [Define if using C++ yescrypt])
fi
```

---

## Timeline

| Week | Tasks | Deliverable |
|------|-------|-------------|
| 1-2 | Phases 2a-2c (Memory, Endian, Salsa20) | Unit tests pass, perf within 5% |
| 3-4 | Phases 2d-2e (PWXForm, SMix) | Integration tests pass |
| 5 | Phase 2f (KDF), Build system | Full C++ variant compiles |
| 6 | Testing & optimization | Regression tests pass, docs complete |

---

## Summary

**Phase 2 transforms the yescrypt implementation from C to pure C++ while maintaining:**

✅ **Correctness** — Test vectors match C implementation  
✅ **Performance** — Benchmarks show <5% overhead  
✅ **Safety** — RAII, bounds-checked spans, no raw pointers  
✅ **Maintainability** — Clear separation of concerns, modern C++17  
✅ **Compatibility** — Legacy C interface remains for Phase 3 integration  

Proceed to Phase 3 once all tests pass and benchmarks are documented.
