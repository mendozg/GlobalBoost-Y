# Phase 3 Detailed Roadmap: File-by-File Optimization Targets

## Overview

This is the concrete, repo-ready roadmap for Phase 3. It lists:
- specific files to create or modify
- proposed optimization targets within each file
- expected performance gains
- validation requirements per file

Use this as the implementation checklist for Phase 3.

---

## File Roadmap Summary

| File | Type | Purpose | Priority | Est. Gain |
|------|------|---------|----------|-----------|
| `yescrypt_profile.hpp` | NEW | Profiling instrumentation | HIGH | baseline data |
| `yescrypt_simd.hpp` | NEW | SIMD dispatch layer | HIGH | 10-20% |
| `yescrypt_simd_sse2.cpp` | NEW | SSE2 intrinsics | HIGH | 8-15% |
| `yescrypt_simd_avx2.cpp` | NEW | AVX2 intrinsics (optional) | MEDIUM | 15-25% |
| `yescrypt_salsa20.cpp` | MODIFY | inline + optimization | HIGH | 5-10% |
| `yescrypt_smix.cpp` | MODIFY | memory reuse + inlining | HIGH | 3-8% |
| `yescrypt_memory.cpp` | MODIFY | pre-allocation strategy | MEDIUM | 2-5% |
| `yescrypt_benchmark.cpp` | NEW | perf regression gates | HIGH | N/A (validation) |
| `CMakeLists.txt` | MODIFY | SIMD detection + flags | MEDIUM | N/A (build) |
| `ci/scripts/perf_regression.yml` | NEW | CI benchmark job | HIGH | N/A (CI) |

---

## Phase 3.1: Profiling Instrumentation

### File: `src/crypto/yescrypt/cpp/yescrypt_profile.hpp`

**Purpose:** lightweight profiling macros to measure hot path performance

**Proposed content:**

```cpp
#pragma once

#include <chrono>
#include <map>
#include <string>
#include <atomic>

namespace yescrypt {
namespace profile {

struct TimingStats {
    uint64_t call_count = 0;
    uint64_t total_ns = 0;
    uint64_t min_ns = UINT64_MAX;
    uint64_t max_ns = 0;
    
    double avg_ns() const {
        return call_count > 0 ? (double)total_ns / call_count : 0.0;
    }
};

class ProfileCollector {
public:
    static ProfileCollector& instance();
    
    void record(const std::string& function, uint64_t elapsed_ns);
    void print_summary() const;
    TimingStats get_stats(const std::string& function) const;
    
private:
    std::map<std::string, TimingStats> stats_;
};

// RAII scoped timer
class ScopedTimer {
public:
    explicit ScopedTimer(const std::string& name);
    ~ScopedTimer();
    
private:
    std::string name_;
    std::chrono::high_resolution_clock::time_point start_;
};

} // namespace profile
} // namespace yescrypt

// Macro for easy instrumentation
#ifdef YESCRYPT_PROFILE
#define YESCRYPT_TIMER(name) yescrypt::profile::ScopedTimer __timer(name)
#else
#define YESCRYPT_TIMER(name) // no-op
#endif
```

**Integration points:**
- wrap `Salsa20::apply_core()` with timer
- wrap `Salsa20::blockmix()` with timer
- wrap `SMix::smix1()` and `SMix::smix2()` with timer
- wrap `KDF::derive()` with timer

**Validation:**
- compile with `-DYESCRYPT_PROFILE`
- run benchmark with profiling enabled
- identify top 3 hottest functions

---

## Phase 3.2: SIMD Dispatch Layer

### File: `src/crypto/yescrypt/cpp/yescrypt_simd.hpp`

**Purpose:** CPU feature detection and function dispatch to SIMD paths

**Proposed content:**

```cpp
#pragma once

#include <cstdint>
#include <array>

namespace yescrypt {
namespace simd {

// CPU feature detection
enum class CPUFeatures : uint32_t {
    NONE = 0,
    SSE2 = 1,
    AVX = 2,
    AVX2 = 4,
    AVX512F = 8
};

class CPUInfo {
public:
    static CPUInfo& instance();
    
    bool has_feature(CPUFeatures feat) const;
    CPUFeatures best_available() const;
    const char* feature_string() const;
    
private:
    CPUFeatures features_ = CPUFeatures::NONE;
    
    void detect_features();
};

// Salsa20 SIMD implementations
class Salsa20Dispatch {
public:
    // Dispatch to best available SIMD variant
    static void apply_core(std::array<uint64_t, 8>& block);
    static void blockmix(
        const std::array<uint64_t, 8>& input,
        std::array<uint64_t, 8>& output,
        size_t r
    );
    
private:
    // Scalar fallback (always available)
    static void apply_core_scalar(std::array<uint64_t, 8>& block);
    
    // SSE2 (x86_64/i386 baseline)
    static void apply_core_sse2(std::array<uint64_t, 8>& block);
    
    // AVX2 (optional, newer CPUs)
    static void apply_core_avx2(std::array<uint64_t, 8>& block);
};

} // namespace simd
} // namespace yescrypt
```

**Integration points:**
- call `Salsa20Dispatch::apply_core()` from `Salsa20::apply_core()`
- call `Salsa20Dispatch::blockmix()` from `Salsa20::blockmix()`
- dispatch based on `CPUInfo::best_available()`

**Validation:**
- test on CPUs without SSE2 (fallback to scalar)
- test on SSE2-capable CPUs
- test on AVX2-capable CPUs (if implemented)
- verify output matches scalar reference

---

## Phase 3.3: SSE2 Intrinsics Implementation

### File: `src/crypto/yescrypt/cpp/yescrypt_simd_sse2.cpp`

**Purpose:** high-performance SSE2 version of Salsa20 hot paths

**Proposed optimizations:**

1. **Salsa20 `apply_core()` with SSE2:**
   - Load 16 x uint32_t into 4 x `__m128i` registers
   - Perform column rounds using `_mm_add_epi32`, `_mm_xor_si128`, rotate via shifts
   - Perform row rounds similarly
   - Store back to output

   Expected gain: 10-15%

2. **BlockMix with pre-allocation:**
   - reuse temporary buffers across calls
   - avoid repeated allocations
   - keep temp state in static or thread_local buffer

   Expected gain: 3-5%

**Key functions to implement:**

```cpp
namespace yescrypt::simd {

void Salsa20Dispatch::apply_core_sse2(std::array<uint64_t, 8>& block) {
    // Load 8 x uint64_t → 4 x __m128i (64-bit pairs)
    // Perform 8 rounds of Salsa20 using SSE2 ops
    // Store back
}

} // namespace simd
```

**Testing:**
- compare output against scalar reference on known vectors
- benchmark single call performance
- benchmark throughput over 1000 iterations

---

## Phase 3.4: AVX2 Intrinsics (Optional)

### File: `src/crypto/yescrypt/cpp/yescrypt_simd_avx2.cpp`

**Purpose:** faster Salsa20 on modern CPUs with AVX2 support

**Proposed optimizations:**

1. **Salsa20 with AVX2:**
   - use 256-bit registers to process 2x blocks in parallel
   - or process 8 x uint32 at once for single block
   - significant throughput improvement on supported CPUs

   Expected gain: 15-25%

2. **Conditional compilation:**
   - only compile if `-mavx2` is available
   - fallback to SSE2 if not supported

**Key functions:**

```cpp
namespace yescrypt::simd {

void Salsa20Dispatch::apply_core_avx2(std::array<uint64_t, 8>& block) {
    // Similar to SSE2 but using 256-bit __m256i registers
    // 2x the throughput for parallel operations
}

} // namespace simd
```

**Note:** This is optional for Phase 3. Start with SSE2, add AVX2 only if benchmark data shows it's worth the complexity.

---

## Phase 3.5: Salsa20 Core Optimization

### File: `src/crypto/yescrypt/cpp/yescrypt_salsa20.cpp` (MODIFY)

**Proposed changes:**

1. **Inline hot helpers:**
   ```cpp
   // Before
   static void unshuffle(...);
   
   // After
   static inline void unshuffle(...)  // Force inline
   ```

2. **Pre-allocate temp buffers:**
   ```cpp
   // Before
   void blockmix(...) {
       std::array<uint64_t, 8> temp;  // allocated every call
   }
   
   // After
   thread_local static std::array<uint64_t, 8> temp_buffer;
   // reuse across calls
   ```

3. **Optimize rotation macro:**
   ```cpp
   // Use constexpr for compile-time folding
   static constexpr uint32_t rotl32(uint32_t x, unsigned n) {
       return (x << n) | (x >> (32 - n));
   }
   ```

4. **Add dispatch to SIMD:**
   ```cpp
   void Salsa20::apply_core(std::array<uint64_t, 8>& block) {
       // Call dispatch instead of inline logic
       Salsa20Dispatch::apply_core(block);
   }
   ```

**Expected gains:**
- thread-local buffer reuse: 2-3%
- inlining: 2-3%
- SIMD dispatch: 10-15%
- total: 14-21%

**Validation:**
- all Salsa20 test vectors pass
- output matches scalar reference
- performance improved on benchmark

---

## Phase 3.6: SMix Memory Reuse Optimization

### File: `src/crypto/yescrypt/cpp/yescrypt_smix.cpp` (MODIFY)

**Proposed changes:**

1. **Scratch buffer pre-allocation:**
   ```cpp
   // Before
   void SMix::compute(...) {
       std::vector<uint64_t> V(N * r * 32 / 8);  // new every time
   }
   
   // After
   // Provide pre-allocated buffer or use Region class
   thread_local static Region scratch_buffer;
   // allocate once, reuse
   ```

2. **Reduce temporary copies:**
   ```cpp
   // Before
   std::vector<uint64_t> temp = input;
   // process temp
   // copy back to output
   
   // After
   // process in-place where possible
   ```

3. **Inline small helper functions:**
   ```cpp
   static inline uint64_t integerify(...)
   static inline uint64_t p2floor(...)
   static inline void xor_block(...)
   ```

4. **Align V buffer to cache line:**
   ```cpp
   // Use Region class with alignment guarantee
   Region V;
   V.allocate(N * r * 32 / 8);  // aligned to 64B
   ```

**Expected gains:**
- scratch reuse: 3-5%
- inlining: 1-2%
- cache alignment: 1-3%
- total: 5-10%

**Validation:**
- test with various N, r, p values
- no memory leaks (ASAN)
- output deterministic

---

## Phase 3.7: Memory & Cache Tuning

### File: `src/crypto/yescrypt/cpp/yescrypt_memory.cpp` (MODIFY)

**Proposed changes:**

1. **Pre-allocation pool:**
   ```cpp
   // Cache frequently-used buffer sizes
   class BufferPool {
       Region pool_2k;   // for N=2048
       Region pool_16k;  // for N=16384
       Region pool_1m;   // for N=1048576
   };
   ```

2. **Verify cache-line alignment:**
   ```cpp
   // Ensure all allocations respect 64B alignment
   // (already done in Phase 2, verify in Phase 3)
   ```

3. **Minimize false sharing:**
   - keep thread-local state truly thread-local
   - verify no shared mutable state in hot paths

**Expected gains:**
- pool reuse: 2-4%
- reduced allocator overhead: 1-2%
- total: 3-6%

**Validation:**
- memory profiling (valgrind)
- no allocator contention (TSan if applicable)
- benchmark shows consistent performance

---

## Phase 3.8: Compiler Optimization Flags

### File: `src/crypto/yescrypt/cpp/CMakeLists.txt` (MODIFY)

**Proposed changes:**

```cmake
# Release mode optimizations
if(CMAKE_BUILD_TYPE MATCHES Release)
    if(MSVC)
        target_compile_options(yescrypt_cpp PRIVATE /O2 /Oi)
    else()
        target_compile_options(yescrypt_cpp PRIVATE -O3 -march=native)
        target_compile_options(yescrypt_cpp PRIVATE -fno-strict-aliasing)
        target_compile_options(yescrypt_cpp PRIVATE -funroll-loops)
    endif()
endif()

# SIMD detection and enablement
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-msse2" COMPILER_SUPPORTS_SSE2)
if(COMPILER_SUPPORTS_SSE2)
    target_compile_options(yescrypt_cpp PRIVATE -msse2)
    target_compile_definitions(yescrypt_cpp PRIVATE YESCRYPT_SIMD_SSE2)
endif()

check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
if(COMPILER_SUPPORTS_AVX2 AND ENABLE_AVX2)
    target_compile_options(yescrypt_cpp PRIVATE -mavx2)
    target_compile_definitions(yescrypt_cpp PRIVATE YESCRYPT_SIMD_AVX2)
endif()

# Link-time optimization (optional)
if(ENABLE_LTO)
    target_compile_options(yescrypt_cpp PRIVATE -flto)
    target_link_options(yescrypt_cpp PRIVATE -flto)
endif()
```

**Expected gains:**
- `-O3`: 5-10%
- `-march=native`: 2-5%
- `-funroll-loops`: 2-3%
- total: 9-18%

**Validation:**
- verify reproducible builds
- check binary size increase
- benchmark on reference hardware

---

## Phase 3.9: Benchmarking Suite

### File: `src/crypto/yescrypt/cpp/yescrypt_benchmark.cpp` (NEW/EXPAND)

**Proposed benchmarks:**

```cpp
// 1. Baseline: default config
BENCHMARK(Yescrypt_Hash_Default)
    ->Iterations(10)
    ->Unit(benchmark::kMillisecond);

// 2. Medium: larger N
BENCHMARK(Yescrypt_Hash_Medium)
    ->Iterations(5)
    ->Unit(benchmark::kMillisecond);

// 3. Large: very large N
BENCHMARK(Yescrypt_Hash_Large)
    ->Iterations(1)
    ->Unit(benchmark::kMillisecond);

// 4. Throughput: many small hashes
BENCHMARK(Yescrypt_Hash_Throughput)
    ->Iterations(1000)
    ->Unit(benchmark::kMicrosecond);
```

**Output format:**
- JSON (for CI parsing)
- human-readable (for manual review)
- comparison with Phase 2 baseline and legacy C

**Success criteria:**
- default config: within ±2% of C baseline
- large config: within ±5% of C baseline
- no outliers or variance >10%

---

## Phase 3.10: CI Performance Regression Gate

### File: `.github/workflows/yescrypt_phase3_perf.yml` (NEW)

**Proposed workflow:**

```yaml
name: Phase 3 Performance Validation

on:
  pull_request:
    paths:
      - 'src/crypto/yescrypt/cpp/**'
  schedule:
    - cron: '0 2 * * *'

jobs:
  benchmark:
    runs-on: ubuntu-latest
    
    steps:
      - uses: actions/checkout@v3
      
      - name: Build with optimizations
        run: |
          cmake -B build \
            -DCMAKE_BUILD_TYPE=Release \
            -DENABLE_BENCHMARKS=ON
          cmake --build build -j$(nproc)
      
      - name: Run benchmarks
        run: |
          ./build/src/test/crypto/yescrypt_benchmark \
            --benchmark_out=current.json \
            --benchmark_out_format=json
      
      - name: Compare with baseline
        run: |
          python3 ci/scripts/phase3_perf_compare.py \
            current.json \
            .github/phase3_perf_baseline.json
      
      - name: Fail if regression detected
        run: |
          python3 ci/scripts/phase3_check_thresholds.py \
            current.json \
            --threshold-default=2 \
            --threshold-large=5
```

**Baseline file:** `.github/phase3_perf_baseline.json`
- captured at end of Phase 2
- updated after Phase 3 optimizations
- never manually edited

---

## Phase 3.11: Profiling Script

### File: `ci/scripts/phase3_profile.sh` (NEW)

**Purpose:** run profiling and generate report for optimization analysis

**Proposed script:**

```bash
#!/bin/bash
set -e

echo "Building with profiling instrumentation..."
cmake -B build_profile \
    -DCMAKE_BUILD_TYPE=Release \
    -DYESCRYPT_PROFILE=ON

cmake --build build_profile -j$(nproc)

echo "Running profiling benchmark..."
YESCRYPT_PROFILE=1 ./build_profile/src/test/crypto/yescrypt_benchmark \
    --benchmark_out=profile_results.json \
    --benchmark_out_format=json

echo "Generating profile report..."
python3 - <<'EOF'
import json

# Parse results and identify hottest functions
with open('profile_results.json') as f:
    data = json.load(f)

# Output summary
for bench in data['benchmarks']:
    print(f"{bench['name']:40s} {bench['real_time']:10.2f} ms")
EOF
```

---

## Phase 3.12: Validation Checklist

### Functional validation
- [ ] all Phase 2 test vectors pass
- [ ] output deterministic under Phase 3 optimizations
- [ ] no behavior change from Phase 2

### Performance validation
- [ ] profiling data collected and reviewed
- [ ] hottest functions identified
- [ ] SIMD paths outperform scalar baseline
- [ ] benchmark gate configured and working
- [ ] release build meets ±2-5% target

### Hardening validation
- [ ] ASAN clean with optimizations enabled
- [ ] UBSAN clean with optimizations enabled
- [ ] no undefined behavior detected
- [ ] no alignment faults
- [ ] no memory leaks

### Build validation
- [ ] compiles on Linux (GCC, Clang)
- [ ] compiles on macOS
- [ ] compiles on Windows (MSVC)
- [ ] no compiler warnings in `-Wall -Wextra`

---

## Phase 3 Implementation Order

### Week 1: Profiling & Baseline
1. Add `yescrypt_profile.hpp`
2. instrument hot functions
3. run profiling build
4. identify top 3 bottlenecks
5. capture Phase 2 baseline

### Week 2: SIMD Dispatch & SSE2
1. create `yescrypt_simd.hpp` (dispatch layer)
2. implement SSE2 path in `yescrypt_simd_sse2.cpp`
3. integrate dispatch into `Salsa20::apply_core()`
4. validate correctness against vectors
5. benchmark and compare

### Week 3: Memory & Compiler Tuning
1. optimize `yescrypt_smix.cpp` for buffer reuse
2. optimize `yescrypt_salsa20.cpp` for inlining
3. update `CMakeLists.txt` with release flags
4. verify memory alignment and cache-line usage

### Week 4: Benchmarking & CI
1. expand `yescrypt_benchmark.cpp` with Phase 3 scenarios
2. add CI job for regression checking
3. validate all thresholds pass
4. document baseline and acceptance criteria

### Week 5: Optional - AVX2 & Polish
1. implement AVX2 variant (if data justifies)
2. stress test with ASAN/UBSAN/TSAN
3. final performance report
4. prepare PR and summary

---

## Success Metrics

### Phase 3 is complete when:

✅ **Correctness preserved**
- all test vectors pass
- output identical to Phase 2
- no behavior changes

✅ **Performance improved**
- release build at least as fast as C baseline
- benchmark gates active and passing
- <2% overhead vs C in default config

✅ **Hardening validated**
- ASAN/UBSAN/TSAN all pass
- no memory safety issues
- platform support verified

✅ **Documentation complete**
- profiling results documented
- optimization decisions justified
- benchmark baseline committed

---

## Summary

This roadmap breaks Phase 3 into 12 concrete files and optimization targets. Follow the order, validate at each step, and let the profiling data guide your optimization priorities.

Do not optimize blindly. Profile first, then optimize the actual bottlenecks.
