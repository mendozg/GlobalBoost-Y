# Phase 3: Yescrypt Performance Optimization & Hardening

## Overview

Phase 3 takes the working pure C++ yescrypt implementation from Phase 2 and turns it into a performance-tuned, production-grade version. The goal is to preserve the exact algorithmic behavior while reducing overhead and matching or exceeding the performance of the legacy C implementation.

This phase is intentionally focused on performance and hardening, not on algorithm redesign. The algorithm and API should remain stable across Phase 2 and Phase 3.

---

## Goals

1. Preserve exact behavior from Phase 2
2. Match or exceed legacy C performance
3. Improve memory locality and reduce temporary allocations
4. Keep ABI/API compatibility intact
5. Add benchmark gating to CI to prevent regressions
6. Maintain stability on supported platforms

---

## Scope

### In Scope
- Profiling hot paths in Salsa20, BlockMix, SMix, and KDF
- SIMD optimization for supported architectures
- Compiler optimization and inlining
- Memory layout and cache tuning
- Benchmarking and CI regression gates
- Platform-specific tuning for x86_64 and ARM
- Hardening and validation under ASAN/UBSAN/TSAN

### Out of Scope
- Algorithm redesign or logic changes
- Breaking compatibility with the Phase 2 API
- Large-scale rewrite of unrelated code
- Unnecessary optimization of unaffected areas

---

## Phase 3 Workstreams

### Workstream A: Profiling & Bottleneck Analysis

Before making any performance changes, identify the real bottlenecks.

**Priority targets:**
- `Salsa20::apply_core()`
- `Salsa20::blockmix()`
- `PWXForm::blockmix()`
- `SMix::smix1()`
- `SMix::smix2()`
- `KDF::derive()`

**Recommended tools:**
- `perf` (Linux)
- Instruments (macOS)
- VTune / AMD uProf / Intel tools (optional)
- Google Benchmark for microbenchmarks

**Success metric:**
- Data-driven decision on which functions justify optimization work.

---

### Workstream B: SIMD Optimization

Use architecture-specific intrinsics only when they materially improve throughput and preserve correctness.

**Targets:**
- x86_64: SSE2, AVX2, AVX-512 where supported
- ARM: NEON where available

**Implementation pattern:**
- Keep scalar fallback path for unsupported architectures
- Optimize only the hottest operations first
- Validate correctness against Phase 2 vectors and regression tests

**Priority order:**
1. `salsa20_8` / `apply_core()`
2. `blockmix_salsa8` / `blockmix()`
3. `block_pwxform()` / `transform_block()`
4. final `SMix` loops

**Note:**
Use SIMD only after verifying that the scalar C++ version is already correct. This avoids optimizing the wrong layer.

---

### Workstream C: Memory & Cache Tuning

The yescrypt algorithm is memory-heavy. Tuning memory locality can produce a meaningful performance benefit.

**Focus areas:**
- align allocations to 64-byte cache boundaries
- reduce repeated buffer copies
- minimize temporary memory churn
- ensure `Region` allocations are reused where possible
- avoid false sharing in multithreaded or parallel code paths

**Examples:**
- If a buffer is being reallocated inside loops, move it to a persistent scratch area
- Reuse `XY`, `V`, and `S` work buffers across repeated hashing calls
- Reduce stack-based copies in hot paths
- Reserve exact sizes rather than resizing repeatedly

**Success metric:**
- Lower memory latency and reduced allocations without changing algorithm semantics

---

### Workstream D: Compiler Optimization

Use compiler features that help the hot path without making the code brittle.

**Recommended optimizations:**
- compile with `-O3` in release builds
- enable `-march=native` for performance builds
- `constexpr` for fixed operations and constants
- `inline` for highly used helper functions
- profile-guided optimization (PGO) where available
- `-flto` only if the build remains stable and reproducible

**Recommended compiler settings:**
- GCC / Clang: `-O3 -DNDEBUG`
- x86_64: `-msse2` or higher, if supported
- optional: `-fno-strict-aliasing` only if needed for correctness

**Success metric:**
- stable release performance improvement without unsafe assumptions

---

### Workstream E: Benchmarking & CI Gates

Phase 3 should add automated performance checks to prevent accidental regressions.

**Benchmark scenarios:**
- default hash path (`N=2048, r=8, p=1`)
- medium memory config (`N=16384, r=8, p=1`)
- large workload (`N=1048576, r=8, p=1`)
- slow path / time parameter variations
- multiple compilers (GCC, Clang, MSVC)

**Acceptance criteria:**
- no >5% regression compared to the Phase 2 baseline
- no >2% regression versus the legacy C implementation in release builds
- if regression exceeds threshold, CI fails

**Benchmark tools:**
- Google Benchmark
- custom Python benchmark runner for CI comparison
- JSON output capture

---

## Proposed File Structure

```text
src/crypto/yescrypt/cpp/
├── yescrypt.hpp
├── yescrypt.cpp
├── yescrypt_memory.hpp
├── yescrypt_memory.cpp
├── yescrypt_endian.hpp
├── yescrypt_salsa20.hpp
├── yescrypt_salsa20.cpp
├── yescrypt_pwxform.hpp
├── yescrypt_pwxform.cpp
├── yescrypt_smix.hpp
├── yescrypt_smix.cpp
├── yescrypt_simd.hpp
├── yescrypt_simd.cpp
├── yescrypt_profile.hpp
├── yescrypt_benchmark.cpp
└── yescrypt_config.hpp
```

---

## Implementation Plan

### Phase 3.1: Baseline & Profiling

**Tasks:**
- build Phase 2 in release mode
- run benchmark suite on reference hardware
- collect call profiles for the hot functions
- confirm whether bottlenecks are truly in arithmetic vs memory

**Deliverable:**
- benchmark report with hotspots and suggested optimization targets

---

### Phase 3.2: SIMD Hot-Path Optimization

**Tasks:**
- identify the most expensive arithmetic loops
- add CPU feature detection
- implement optimized SSE2 / AVX2 path behind runtime dispatch
- keep scalar fallback path active
- validate against deterministic test vectors

**Deliverable:**
- optimized intrinsics path for supported CPUs

---

### Phase 3.3: Memory & Allocation Tuning

**Tasks:**
- align memory regions with cache lines
- remove avoidable copies and repeated allocations
- reuse scratch buffers where possible
- verify all allocations remain valid under ASAN/UBSAN

**Deliverable:**
- reduced allocation churn and improved cache-locality

---

### Phase 3.4: Compiler & Release Tuning

**Tasks:**
- enable full optimized release settings
- inline hot helpers and reduce abstraction overhead
- tune for the target architecture
- verify release build stability and correctness

**Deliverable:**
- optimized release binaries with reproducible performance

---

### Phase 3.5: Benchmark Enforcement & CI

**Tasks:**
- add benchmark jobs to CI
- compare to baseline and fail on regression thresholds
- add artifact upload for benchmark results
- document thresholds and interpretation

**Deliverable:**
- automated benchmark gate and performance regression reporting

---

## Benchmark Expectations

Target performance goals:
- Phase 3 should be at least as fast as the legacy C implementation in release builds
- ideally faster in optimized environments
- no algorithmic regression under any supported config

### Example success thresholds
- default config: within ±2% of C baseline
- large N config: within ±5% of C baseline
- no correctness regressions on known vectors
- no memory safety regressions

---

## Validation Matrix

### Functional Validation
- [ ] all Phase 2 tests pass
- [ ] all yescrypt vectors pass
- [ ] deterministic output preserved
- [ ] no change in hash semantics

### Security & Hardening Validation
- [ ] ASAN clean
- [ ] UBSAN clean
- [ ] TSan clean when applicable
- [ ] no undefined behavior in optimized builds
- [ ] no alignment faults

### Performance Validation
- [ ] benchmark suite runs successfully
- [ ] release build meets thresholds
- [ ] no regressions across CI platforms

---

## Example Phase 3 PR Summary

> Optimized the pure C++ yescrypt implementation for release builds by profiling the hot paths, introducing architecture-aware SIMD acceleration for Salsa20/BlockMix, and reducing allocation churn via aligned scratch buffers and reuse. The behavior remains consistent with the Phase 2 implementation, and benchmark gates are added to prevent performance regressions in CI.

---

## Suggested PR Checklist

### Code
- [ ] Hot path profiled and validated
- [ ] SIMD optimized path implemented
- [ ] Fallback scalar path retained
- [ ] Memory layout tuned
- [ ] No behavior drift from Phase 2

### Build / CI
- [ ] Release optimization enabled
- [ ] benchmark suite passes
- [ ] no CI regressions
- [ ] ASAN/UBSAN/TSAN checks pass

### Performance
- [ ] no >5% regression versus Phase 2 baseline
- [ ] no >2% regression vs legacy C in release build
- [ ] benchmark artifacts uploaded for review

---

## Risks and Mitigations

### Risk: SIMD introduces errors
**Mitigation:** keep scalar fallback path and validate with large deterministic vectors under multiple compilers.

### Risk: optimization changes algorithm behavior
**Mitigation:** maintain a strict comparison suite against Phase 2 and the original C version.

### Risk: benchmark noise misleads optimization work
**Mitigation:** run benchmarks multiple times and record environment details.

### Risk: performance gains are not portable
**Mitigation:** use feature detection and architecture-specific dispatch.

---

## Final Success Criteria

Phase 3 is complete when all of the following are true:

- Phase 2 behavior is preserved exactly
- Release build performs as well as or better than the legacy C implementation
- Benchmark gates are active in CI
- All security checks pass
- Public API remains stable
- The optimized implementation is ready for production review

---

## Phase 3 Next Actions

1. Capture Phase 2 baseline benchmarks
2. Profile hot functions
3. Optimize the hottest arithmetic loops first
4. Add performance gates to CI
5. Validate against known vectors and legacy output
6. Open PR for review

---

## Summary

Phase 3 is the performance and hardening phase. It is not about changing the algorithm. It is about making the correct Phase 2 C++ implementation fast enough, stable enough, and benchmark-validated enough to be used in production.
