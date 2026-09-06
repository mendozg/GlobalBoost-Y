# Phase 1: Yescrypt C++ Wrapper - Pull Request Template

## Description

### Overview
This pull request implements **Phase 1 of the Yescrypt C-to-C++ migration**: a clean wrapper layer around the existing C implementation with comprehensive CI/CD automation.

**Phase 1 Scope:**
- ✅ C++ wrapper headers and implementations
- ✅ CMake/Makefile integration
- ✅ GitHub Actions workflows (build, test, perf)
- ✅ Unit & regression test suites
- ✅ Performance baseline establishment

**Does NOT include** (Phase 2+):
- Pure C++ algorithm rewrite
- SIMD optimizations
- Memory layout restructuring

### Related Issues
Closes #[issue-number]
Related to #[phase2-issue]

---

## Changes

### Code Changes
- [ ] `src/crypto/yescrypt/cpp/yescrypt.hpp` — C++ wrapper header
- [ ] `src/crypto/yescrypt/cpp/yescrypt.cpp` — Implementation
- [ ] `src/crypto/yescrypt/cpp/yescrypt_config.hpp` — Config header
- [ ] `src/crypto/yescrypt/cpp/CMakeLists.txt` — CMake for wrapper
- [ ] Updated `src/crypto/yescrypt/CMakeLists.txt` — Link wrapper
- [ ] Updated `src/hash.h` — Export C++ interface

### Build System
- [ ] CMake C++17 detection
- [ ] SIMD flag detection (`-msse2`)
- [ ] Cross-platform compatibility (Linux, macOS, Windows)
- [ ] Debug/Release modes supported

### CI/CD Workflows
- [ ] `.github/workflows/yescrypt_build.yml` — Multi-platform builds
- [ ] `.github/workflows/yescrypt_test.yml` — Unit & regression tests
- [ ] `.github/workflows/yescrypt_perf.yml` — Performance benchmarking
- [ ] `.github/workflows/yescrypt_sanitizers.yml` — ASAN/UBSAN/TSan

### Tests
- [ ] `test/unit/crypto/yescrypt_tests.cpp` — Boost.Test suite
- [ ] `test/functional/test_yescrypt_compat.py` — C vs C++ regression
- [ ] Known test vectors included
- [ ] Memory safety tests (ASAN)

### Performance & Benchmarking
- [ ] `ci/scripts/bench_yescrypt.sh` — Local benchmark script
- [ ] `ci/scripts/perf_compare.py` — Baseline comparison
- [ ] `.github/perf_baseline.json` — Performance baseline

### Documentation
- [ ] `YESCRYPT_MIGRATION_PHASE1_CICD.md` — Complete guide
- [ ] Code comments (doxygen-style)
- [ ] Build instructions
- [ ] Test instructions

---

## Testing

### Pre-Submission Checklist

**Build Verification:**
```bash
# Single-platform build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```
- [ ] Builds on Linux
- [ ] Builds on macOS  
- [ ] Builds on Windows

**Unit Tests:**
```bash
ctest --build build --output-on-failure
```
- [ ] All unit tests pass
- [ ] No compiler warnings

**Regression Tests:**
```bash
python3 -m pytest test/functional/test_yescrypt_compat.py -v
```
- [ ] C implementation output matches C++ wrapper
- [ ] All test vectors pass
- [ ] No output divergence

**Memory Safety:**
```bash
cmake -B build -DCMAKE_CXX_FLAGS="-fsanitize=address -g"
cmake --build build
ASAN_OPTIONS=detect_leaks=1 ctest --build build
```
- [ ] No memory leaks
- [ ] No heap corruption
- [ ] No use-after-free

**Undefined Behavior:**
```bash
cmake -B build -DCMAKE_CXX_FLAGS="-fsanitize=undefined -g"
cmake --build build
ctest --build build
```
- [ ] No UB detected
- [ ] No integer overflow
- [ ] No alignment issues

**Code Quality:**
```bash
clang-format --dry-run --Werror src/crypto/yescrypt/cpp/*.cpp
clang-tidy src/crypto/yescrypt/cpp/yescrypt.cpp
```
- [ ] Code style consistent
- [ ] No clang-tidy warnings
- [ ] Coverage >90%

---

## Performance Impact

### Benchmarking Results

**Local Benchmark (before):**
```
Yescrypt_Hash:     [baseline_time] ms/op
```

**Local Benchmark (after):**
```
Yescrypt_Hash:     [new_time] ms/op
Overhead:          [overhead]%
```

**Expected Impact:**
- ✅ Overhead <5% vs C implementation
- ✅ Thread-local caching working
- ✅ No memory bloat (binary size <5MB increase)

**Test Results:**
```
Platform          Time (ms)    vs Baseline    Status
├─ Linux GCC      X.XXX        +2.3%         ✓ PASS
├─ Linux Clang    X.XXX        +1.8%         ✓ PASS
├─ macOS Clang    X.XXX        +3.1%         ✓ PASS
└─ Windows MSVC   X.XXX        +2.9%         ✓ PASS
```

---

## Backwards Compatibility

### API Compatibility
- [ ] Existing C interface unchanged (`yescrypt_hash`)
- [ ] C code links without modification
- [ ] No breaking changes to public headers

### ABI Compatibility
- [ ] Binary-compatible with previous version
- [ ] No symbol changes
- [ ] No typedef changes in public API

### Upgrade Path
```cpp
// Old code (still works)
extern "C" void yescrypt_hash(const char *input, char *output);
yescrypt_hash(input, output);

// New C++ code (opt-in)
using namespace yescrypt;
std::array<uint8_t, 80> in;
std::array<uint8_t, 32> out;
Yescrypt::hash(in, out);
```

---

## Review Checklist

### Code Review
- [ ] Wrapper correctly calls C implementation
- [ ] Error handling is comprehensive
- [ ] Thread-safety verified (thread_local usage correct)
- [ ] No circular dependencies
- [ ] RAII principles followed

### Security Review
- [ ] Input validation present
- [ ] Buffer overflows impossible (bounds checking)
- [ ] Timing attacks mitigated (constant-time where needed)
- [ ] No hardcoded secrets or keys

### Performance Review
- [ ] Profiling done (no unexpected branches)
- [ ] Inlining opportunities identified
- [ ] Memory usage acceptable
- [ ] Cache-line alignment verified

### Documentation Review
- [ ] API documentation complete (doxygen)
- [ ] Examples provided
- [ ] Known limitations documented
- [ ] Migration guide included (for Phase 2)

### Test Coverage
- [ ] Unit test coverage >90%
- [ ] All edge cases tested
- [ ] Platform-specific code tested
- [ ] Sanitizers pass (ASAN/UBSAN/TSan)

---

## CI/CD Status

### GitHub Actions Status
- [ ] `yescrypt_build.yml` — ✓ Passing
  - [ ] Ubuntu + gcc
  - [ ] Ubuntu + clang
  - [ ] macOS + clang
  - [ ] Windows + MSVC
  
- [ ] `yescrypt_test.yml` — ✓ Passing
  - [ ] Unit tests
  - [ ] Regression tests
  - [ ] Memory safety (ASAN)
  
- [ ] `yescrypt_perf.yml` — ✓ Baseline established
  - [ ] No regressions
  - [ ] Results documented

### Linting & Style
- [ ] clang-format clean
- [ ] clang-tidy warnings addressed
- [ ] cpplint passes
- [ ] No compiler warnings (with `-Wall -Wextra`)

---

## Migration Path

### This Phase (Phase 1)
```
C implementation (working)
         ↓
    C++ Wrapper (Phase 1) ← YOU ARE HERE
         ↓
    Phase 2: Pure C++ Algorithm
         ↓
    Phase 3: Performance Optimization
         ↓
    Phase 4: Full Blockchain Integration
```

### Next Phase (Phase 2)
- [ ] Issue created for Phase 2 work
- [ ] Phase 2 guide documented at `YESCRYPT_MIGRATION_PHASE2.md`
- [ ] Design review completed
- [ ] Not blocked by Phase 1 approval

---

## Deployment Notes

### Installation
```bash
# From source
./autogen.sh
./configure --enable-yescrypt-cpp
make -j$(nproc)
make install

# From binary
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build
```

### Configuration
```bash
# Optional: Build without C++ wrapper
./configure --disable-yescrypt-cpp

# Optional: Enable debug symbols
cmake -B build -DCMAKE_BUILD_TYPE=Debug
```

### Rollback Plan
If issues discovered post-merge:
1. Disable C++ wrapper at compile time
2. Code falls back to C-only implementation
3. No blockchain impact
4. Zero downtime deployment

---

## Additional Notes

### Known Limitations
- Phase 1 is a **thin wrapper**, not a pure C++ rewrite
- Performance overhead <5% is acceptable for Phase 1
- Full C++ algorithm migration happens in Phase 2

### Future Improvements
- [ ] Phase 2: Pure C++ algorithm
- [ ] Phase 3: Advanced SIMD optimizations
- [ ] Phase 4: Parallel processing (OpenMP)

### Questions?
See [YESCRYPT_MIGRATION_PHASE1_CICD.md](../YESCRYPT_MIGRATION_PHASE1_CICD.md) for:
- Complete implementation details
- Test suite documentation
- CI/CD workflow explanations
- Troubleshooting guide

---

## Approvals Required

- [ ] Code Review (Core Maintainer)
- [ ] Performance Review (Performance Lead)
- [ ] Security Review (Security Team)
- [ ] Integration Test (Release Manager)

---

## Merge Checklist

Before merging, verify:
- [ ] All CI checks passing
- [ ] Code review approved
- [ ] No blocking issues
- [ ] Performance baseline established
- [ ] Documentation complete
- [ ] Phase 2 issue created

---

## PR Metadata

**Type:** Feature / Refactoring  
**Priority:** High  
**Complexity:** Medium  
**Risk Level:** Low (backwards compatible)  
**Testing Coverage:** High (>90%)  
**Estimated Reviewer Time:** 2-3 hours  
**Estimated Merge Time:** 1 day  

---

**Template Version:** 1.0  
**Created:** 2026-09-05  
**Last Updated:** 2026-09-05
