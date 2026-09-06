# Phase 2: Yescrypt Pure C++ Algorithm Migration - Pull Request Template

## Description

### Overview
This pull request implements **Phase 2 of the Yescrypt C-to-C++ migration**: complete rewrite of the yescrypt algorithm in pure C++ with modern language features and improved maintainability.

**Phase 2 Scope:**
- ✅ Pure C++ implementation (no C code in algorithms)
- ✅ RAII memory management (no manual malloc/free)
- ✅ Type-safe spans (gsl::span instead of raw pointers)
- ✅ Compile-time optimizations (constexpr, static configs)
- ✅ Sub-phase decomposition (Memory, Endian, Salsa20, PWXForm, SMix, KDF)
- ✅ Performance parity with C (<5% overhead)
- ✅ Full test vector validation

**Does NOT include** (Phase 3+):
- SIMD optimizations (use existing SSE2 from C)
- Advanced parallelization (Phase 4)
- Performance tuning beyond parity

### Related Issues
Closes #[phase2-issue-number]
Depends on #[phase1-pr-number] (Phase 1 must be merged first)

### Prerequisites
- [ ] Phase 1 (C++ wrapper) merged and stable
- [ ] Performance baseline established
- [ ] Code review template approved
- [ ] Test infrastructure validated

---

## Changes

### Sub-Phase 2a: Memory Layer
- [ ] `src/crypto/yescrypt/cpp/yescrypt_memory.hpp` — Region RAII class
- [ ] `src/crypto/yescrypt/cpp/yescrypt_memory.cpp` — Implementation with alignment
- [ ] Move semantics, copy deleted
- [ ] Cache-line alignment (64B)
- [ ] Automatic cleanup on scope exit

### Sub-Phase 2b: Endianness & Utilities
- [ ] `src/crypto/yescrypt/cpp/yescrypt_endian.hpp` — Endianness helpers
- [ ] Constexpr endian detection (`std::endian`)
- [ ] Compile-time byte order conversions
- [ ] 32-bit & 64-bit variants (le32dec, be64enc, etc.)

### Sub-Phase 2c: Cryptographic Primitives
- [ ] `src/crypto/yescrypt/cpp/yescrypt_salsa20.hpp` — Salsa20 core
- [ ] `src/crypto/yescrypt/cpp/yescrypt_salsa20.cpp` — Implementation
- [ ] `apply_core()` — 8-round Salsa20/8
- [ ] `blockmix()` — BlockMix operation
- [ ] SIMD shuffle/unshuffle operations

### Sub-Phase 2d: Block Transform
- [ ] `src/crypto/yescrypt/cpp/yescrypt_pwxform.hpp` — PWXForm S-box
- [ ] `src/crypto/yescrypt/cpp/yescrypt_pwxform.cpp` — Implementation
- [ ] Compile-time S-box configuration
- [ ] `transform_block()` — Apply transformation
- [ ] `blockmix_pwxform()` — PWXForm blockmix

### Sub-Phase 2e: Core Algorithm
- [ ] `src/crypto/yescrypt/cpp/yescrypt_smix.hpp` — SMix declaration
- [ ] `src/crypto/yescrypt/cpp/yescrypt_smix.cpp` — Full implementation
- [ ] `smix1()` — First mixing loop
- [ ] `smix2()` — Second mixing loop
- [ ] `compute()` — Orchestration with parallelism
- [ ] Thread-safe state management

### Sub-Phase 2f: Public KDF Interface
- [ ] `src/crypto/yescrypt/cpp/yescrypt_kdf.hpp` — KDF wrapper
- [ ] `src/crypto/yescrypt/cpp/yescrypt_kdf.cpp` — Implementation
- [ ] Full `yescrypt_kdf()` interface
- [ ] Parameter validation
- [ ] Configuration support (N, r, p, t, flags)
- [ ] SHA-256 pre/post-hashing

### Build System Updates
- [ ] Updated `src/crypto/yescrypt/cpp/CMakeLists.txt` — Add Phase 2 sources
- [ ] Feature detection (C++17, gsl::span availability)
- [ ] Fallback if GSL not available
- [ ] SIMD conditional compilation

### Testing
- [ ] `test/unit/crypto/yescrypt_phase2_tests.cpp` — Unit tests
- [ ] `test/functional/test_yescrypt_phase2_compat.py` — Full regression
- [ ] Known test vectors (RFC 7914 scrypt)
- [ ] Memory-hard property validation
- [ ] Parameter boundary tests

### Documentation
- [ ] `YESCRYPT_MIGRATION_PHASE2.md` — Already provided in Phase 1
- [ ] Code documentation (doxygen-style comments)
- [ ] Algorithm explanation in comments
- [ ] Design decisions documented

---

## Sub-Phase Details & Checklist

### 2a: Memory Management (Week 1)
- [ ] `Region::allocate()` with alignment
- [ ] `Region::deallocate()` via RAII
- [ ] Move constructor/assignment
- [ ] Copy deleted (explicit)
- [ ] Operator overloads (uint8_t*, const uint8_t*)
- [ ] Unit tests pass (test_memory_*.cpp)
- [ ] No ASAN warnings

### 2b: Endianness Utilities (Week 1)
- [ ] Compile-time `is_big_endian()`
- [ ] `le32dec()`, `le32enc()` (32-bit)
- [ ] `le64dec()`, `le64enc()` (64-bit)
- [ ] `be32dec()`, `be32enc()` (big-endian)
- [ ] `be64dec()`, `be64enc()` (big-endian)
- [ ] Constexpr where possible
- [ ] Tests on big-endian systems (if available)

### 2c: Salsa20 Core (Week 2)
- [ ] `Salsa20::apply_core()` — Full 8-round implementation
- [ ] Rotate-left macro as `constexpr`
- [ ] `unshuffle()` — SIMD layout to scalar
- [ ] `shuffle_and_add()` — Scalar back to SIMD
- [ ] `blockmix()` — Interleaved block processing
- [ ] Test vectors from reference implementation
- [ ] Performance within 2% of C version

### 2d: PWXForm (Week 2)
- [ ] S-box configuration struct
- [ ] `Config::BITS`, `Config::SIMD`, `Config::P`, etc.
- [ ] `transform_block()` — Apply S-box transformation
- [ ] Index masking and bounds checking
- [ ] `blockmix_pwxform()` — Full blockmix with PWXForm
- [ ] Partial block handling
- [ ] Unit tests with known inputs

### 2e: SMix Core (Week 3)
- [ ] `smix1()` — First loop with NROM handling
- [ ] `smix2()` — Second loop with RW updates
- [ ] `compute()` — Top-level orchestration
- [ ] Nloop calculations
- [ ] Parallelism support (optional, Phase 1)
- [ ] Thread-safe state isolation
- [ ] Integration with Memory & Salsa20 layers

### 2f: KDF Interface (Week 3)
- [ ] `KDF::kdf()` — Full yescrypt_kdf
- [ ] `KDF::hash()` — Legacy hash wrapper
- [ ] Parameter validation (all checks from C version)
- [ ] SHA-256 pre-hashing for SCRAM compatibility
- [ ] PBKDF2 calls (via OpenSSL or internal)
- [ ] Memory allocation with error recovery
- [ ] Thread-local state management

### Build & Integration (Week 4)
- [ ] All CMake targets compile without warnings
- [ ] C++17 standard compliance verified
- [ ] GSL header availability checked
- [ ] Fallback implementations if needed
- [ ] No circular dependencies
- [ ] Symbol export correct

---

## Testing

### Pre-Submission Checklist

**Build Verification:**
```bash
# Clean build
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Release -DENABLE_PHASE2=ON
cmake --build build -j$(nproc)
```
- [ ] Builds on Linux (gcc, clang)
- [ ] Builds on macOS (clang)
- [ ] Builds on Windows (MSVC)
- [ ] Zero compiler warnings (`-Wall -Wextra -Werror`)

**Unit Tests (Phase 2 specific):**
```bash
ctest --build build --output-on-failure -L "Phase2"
```
- [ ] Memory layer tests pass
- [ ] Endianness tests pass
- [ ] Salsa20 tests pass
- [ ] PWXForm tests pass
- [ ] SMix tests pass
- [ ] KDF tests pass
- [ ] All 6 sub-phase suites pass

**Regression Tests (C vs Pure C++):**
```bash
python3 -m pytest test/functional/test_yescrypt_phase2_compat.py -v
```
- [ ] Output identical to C version
- [ ] All RFC 7914 scrypt vectors pass
- [ ] Known yescrypt vectors pass
- [ ] Memory-hard property verified
- [ ] Parameter boundary tests pass

**Memory Safety:**
```bash
cmake -B build -DCMAKE_CXX_FLAGS="-fsanitize=address -g" -DENABLE_PHASE2=ON
cmake --build build
ASAN_OPTIONS=detect_leaks=1 ctest --build build
```
- [ ] No memory leaks
- [ ] No heap-buffer-overflows
- [ ] No use-after-free
- [ ] Alignment verified
- [ ] Region cleanup correct

**Undefined Behavior Detection:**
```bash
cmake -B build -DCMAKE_CXX_FLAGS="-fsanitize=undefined -g" -DENABLE_PHASE2=ON
cmake --build build
ctest --build build
```
- [ ] No integer overflows
- [ ] No signed integer overflow
- [ ] No alignment violations
- [ ] No division by zero
- [ ] No enum violations

**Thread Safety:**
```bash
cmake -B build -DCMAKE_CXX_FLAGS="-fsanitize=thread -g" -DENABLE_PHASE2=ON
cmake --build build
ctest --build build
```
- [ ] No data races
- [ ] thread_local correctly used
- [ ] Shared state protected
- [ ] No concurrent access issues

**Code Quality:**
```bash
clang-format -i src/crypto/yescrypt/cpp/*.{hpp,cpp}
clang-tidy src/crypto/yescrypt/cpp/*.cpp -- -std=c++17
```
- [ ] Code style consistent (run clang-format)
- [ ] No clang-tidy warnings
- [ ] No cpplint violations
- [ ] Coverage >95% (for Phase 2 code)

**Performance Validation:**
```bash
./ci/scripts/bench_yescrypt.sh
python3 ci/scripts/perf_compare.py benchmark_results.json
```
- [ ] Performance within 5% of Phase 1 C wrapper
- [ ] No regressions vs baseline
- [ ] Thread-local caching verified
- [ ] Memory usage acceptable
- [ ] Binary size increase <10% (vs Phase 1)

---

## Performance Impact

### Benchmarking Requirements

**Phase 1 Baseline (C wrapper):**
```
Yescrypt_Hash:     X.XXX ms/op
```

**Phase 2 Target (Pure C++):**
```
Yescrypt_Hash:     X.YYY ms/op
Target Overhead:   <5%
```

**Breakdown by Sub-Phase:**
```
Memory Layer:      <1% overhead (likely gains from RAII)
Salsa20:           <2% overhead (inlining helps)
PWXForm:           <1% overhead
SMix:              <1% overhead
KDF:               <1% overhead
─────────────────
Total:             <5% overhead ✓
```

**Platform-Specific Results:**
```
Linux GCC 11:      [time] ms/op    [overhead]%
Linux Clang 14:    [time] ms/op    [overhead]%
macOS Clang:       [time] ms/op    [overhead]%
Windows MSVC 2022: [time] ms/op    [overhead]%
```

**Memory Profile:**
```
Stack usage:       <16KB per call (vs C: ~32KB from stack arrays)
Heap usage:        ~10MB for N=2048 (same as C)
Binary size:       <200KB increase (Phase 2 C++ code)
```

---

## Validation & Correctness

### Test Vector Coverage

**RFC 7914 Scrypt Vectors (3 official):**
- [ ] Vector 1: N=16, r=1, p=1 (interactive use)
- [ ] Vector 2: N=1048576, r=8, p=16 (sensitive data)
- [ ] All match reference implementation

**GlobalBoost Yescrypt Vectors:**
- [ ] Default: N=2048, r=8, p=1, t=0, flags=RW|PWXFORM
- [ ] Match C implementation byte-for-byte
- [ ] Consistency across 100+ runs (deterministic)

**Edge Cases:**
- [ ] N = 2 (minimum valid)
- [ ] N = 2^20 (large memory)
- [ ] r = 1 (minimum)
- [ ] r = 32 (maximum practical)
- [ ] p = 1 to p = 16 (various parallelism)
- [ ] t = 0 to t = 10 (various time costs)

**Boundary Conditions:**
- [ ] Empty password (valid, though unusual)
- [ ] 1-byte salt (minimum)
- [ ] 1MB salt (maximum practical)
- [ ] Maximum buffer length (2^32 - 1) * 32

---

## Backwards Compatibility

### API Compatibility
- [ ] `Yescrypt::hash()` works identically to Phase 1
- [ ] `Yescrypt::kdf()` implements full yescrypt_kdf
- [ ] `KDFConfig` structure unchanged
- [ ] Error codes match C version
- [ ] All flags supported (RW, PARALLEL_SMIX, PWXFORM, etc.)

### ABI Compatibility
- [ ] Symbol names unchanged
- [ ] Function signatures compatible
- [ ] typedef sizes match
- [ ] Enum values match

### C Integration
- [ ] C code can call `Yescrypt::hash()` (via extern C wrapper)
- [ ] No breaking changes to existing C interface
- [ ] Falls back gracefully if Phase 2 disabled

### Migration Path
```cpp
// Phase 1 code (still works)
yescrypt::Yescrypt::hash(input, output);

// Phase 2 code (same interface)
yescrypt::Yescrypt::hash(input, output);  // No changes needed!

// Advanced Phase 2 features (new)
yescrypt::KDFConfig cfg{.N = 4096, .r = 16};
yescrypt::Yescrypt::kdf(password, salt, cfg, output, 32);
```

---

## Code Review Checklist

### Algorithm Correctness
- [ ] Salsa20/8 rounds match specification
- [ ] BlockMix interleaving correct
- [ ] SMix loops implement algorithm correctly
- [ ] Integerify calculation accurate
- [ ] Wrap function correct
- [ ] All test vectors pass

### C++ Best Practices
- [ ] RAII used (no manual cleanup)
- [ ] Move semantics where appropriate
- [ ] const-correctness throughout
- [ ] No raw pointers in public API
- [ ] gsl::span for bounds safety
- [ ] Exceptions used for errors

### Memory Safety
- [ ] Buffer overflows impossible
- [ ] Stack overflow prevented
- [ ] Alignment requirements met
- [ ] No uninitialized variables
- [ ] Resource cleanup guaranteed
- [ ] ASAN/UBSAN/TSan pass

### Performance
- [ ] No unnecessary allocations
- [ ] Cache-line alignment used
- [ ] Inline candidates marked
- [ ] Constexpr where possible
- [ ] No virtual calls in hot path
- [ ] Benchmarks show <5% overhead

### Security
- [ ] Input validation comprehensive
- [ ] Parameter bounds checked
- [ ] Memory zeroization on cleanup
- [ ] Constant-time where required
- [ ] No timing side-channels
- [ ] No information leaks

### Documentation
- [ ] Doxygen comments complete
- [ ] Algorithm steps commented
- [ ] Design decisions explained
- [ ] Known limitations noted
- [ ] Examples provided

---

## CI/CD Status

### GitHub Actions Workflows
- [ ] `yescrypt_build.yml` — All platforms pass
  - [ ] Linux GCC 11
  - [ ] Linux Clang 14
  - [ ] macOS Clang
  - [ ] Windows MSVC
  
- [ ] `yescrypt_test.yml` — All tests pass
  - [ ] Phase 2 unit tests
  - [ ] Phase 2 regression tests
  - [ ] Memory safety (ASAN)
  - [ ] UB detection (UBSAN)
  
- [ ] `yescrypt_perf.yml` — Performance validated
  - [ ] <5% overhead
  - [ ] No regressions
  - [ ] Results documented

- [ ] `yescrypt_sanitizers.yml` — Sanitizers clean
  - [ ] AddressSanitizer
  - [ ] UndefinedBehaviorSanitizer
  - [ ] ThreadSanitizer (if applicable)

### Code Quality
- [ ] clang-format passes
- [ ] clang-tidy clean
- [ ] cpplint clean
- [ ] Coverage >95% (Phase 2 code)
- [ ] No compiler warnings

### Documentation Build
- [ ] Doxygen builds without warnings
- [ ] All classes documented
- [ ] All functions documented
- [ ] Examples compile and run

---

## Migration Guide & Phase 3 Planning

### For Developers Using Phase 1
No changes required! Phase 1 API still works identically.

### For Developers Using Phase 2
New advanced features available:
```cpp
// Custom parameters
yescrypt::KDFConfig cfg;
cfg.N = 4096;
cfg.r = 16;
cfg.flags = yescrypt::Flags::RW | yescrypt::Flags::PWXFORM;

std::vector<uint8_t> derived;
yescrypt::Yescrypt::kdf(password, salt, cfg, derived, 64);
```

### Phase 3 (Not This PR)
- [ ] Advanced SIMD (AVX2, AVX-512)
- [ ] Multi-threaded parallelism (OpenMP)
- [ ] Cache optimization
- [ ] Performance tuning

### Phase 4 (Not This PR)
- [ ] Full blockchain integration
- [ ] Mining pool compatibility
- [ ] Consensus validation

---

## Known Limitations

### Phase 2 Scope Limitations
1. **SIMD:** Uses existing C implementation's SIMD
   - Phase 3 will rewrite with native C++ SIMD
   
2. **Parallelism:** Limited to single thread
   - Phase 4 will add OpenMP/threading
   
3. **Performance:** ~5% overhead acceptable for Phase 2
   - Phase 3 will optimize to match or exceed C

### Implementation Notes
- GSL (Guidelines Support Library) required for gsl::span
  - Fallback to raw pointers if GSL unavailable
  - Header-only, no runtime dependency
  
- Requires C++17 standard
  - Fallback to C version if older compiler
  - CMake detects and adapts

---

## Deployment & Rollback

### Deployment Steps
```bash
# 1. Build with Phase 2
cmake -B build -DCMAKE_BUILD_TYPE=Release -DENABLE_PHASE2=ON
cmake --build build -j$(nproc)

# 2. Run all tests
ctest --build build --output-on-failure

# 3. Deploy (standard process)
make install
```

### Feature Flags
```cmake
# Enable/disable Phase 2
-DENABLE_PHASE2=ON    # Use pure C++
-DENABLE_PHASE2=OFF   # Use C wrapper only
```

### Rollback Plan
If critical issues found:
1. Set `-DENABLE_PHASE2=OFF`
2. Rebuild
3. Redeploy
4. Falls back to Phase 1 wrapper immediately
5. Zero downtime (no blockchain impact)

---

## Approvals Required

- [ ] Code Review — Algorithm correctness, C++ style
- [ ] Performance Review — Benchmark validation, overhead check
- [ ] Security Review — Bounds checking, side-channels
- [ ] Integration Test — Full blockchain validation
- [ ] Release Manager — Deployment checklist

---

## Merge Checklist

Before merging to 25.x:
- [ ] All CI checks passing (all platforms)
- [ ] All sanitizers passing (ASAN, UBSAN, TSan)
- [ ] Performance regression check passed
- [ ] Code review approved
- [ ] Security review approved
- [ ] Test coverage >95%
- [ ] Documentation complete
- [ ] Phase 3 issue created
- [ ] Release notes prepared

---

## PR Metadata

**Type:** Major Feature / Refactoring  
**Priority:** High  
**Complexity:** Very High  
**Risk Level:** Medium (algorithm rewrite, but well-tested)  
**Testing Coverage:** Very High (>95%)  
**Estimated Reviewer Time:** 4-5 hours  
**Estimated Merge Time:** 2-3 days  
**Breaking Changes:** None (backwards compatible)  
**Deployment Risk:** Low (feature flag allows rollback)  

---

**Template Version:** 2.0  
**Created:** 2026-09-06  
**Phase 2 Guide:** See `YESCRYPT_MIGRATION_PHASE2.md`
