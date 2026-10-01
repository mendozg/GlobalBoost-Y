# Yescrypt Migration Developer Quick Start

## Overview

This guide is the practical companion to the Phase 1, Phase 2, and Phase 3 migration documents. It is intended for developers joining or continuing the yescrypt C-to-C++ migration effort.

Use this as the single reference for:
- where to start
- what phase does what
- which files are in scope
- what to validate before merging
- how to keep the migration safe and incremental

---

## Migration Roadmap

### Phase 1: Wrapper + CI/CD
**Purpose:** make the existing C implementation safe to integrate into a C++ codebase and automate validation.

**Primary outcomes:**
- C++ wrapper around legacy yescrypt C API
- build system integration
- CI workflows for compile/test/performance
- baseline regression checks
- no algorithm rewrite yet

**Files to review:**
- `YESCRYPT_MIGRATION_PHASE1_CICD.md`
- `.github/pull_request_template_phase1.md`

---

### Phase 2: Pure C++ Port
**Purpose:** rewrite the algorithm in modern C++ while preserving behavior.

**Primary outcomes:**
- RAII memory handling
- compile-time configuration
- endianness utilities
- Salsa20 block logic
- PWXForm and SMix
- final KDF interface

**Files to review:**
- `YESCRYPT_MIGRATION_PHASE2.md`
- `.github/pull_request_template_phase2.md`

---

### Phase 3: Optimization & Hardening
**Purpose:** make the Phase 2 implementation production-grade and performance-competitive with the original C version.

**Primary outcomes:**
- profiling and bottleneck reduction
- SIMD acceleration for supported CPUs
- cache/memory tuning
- release-optimized builds
- benchmark gating in CI

**Files to review:**
- `YESCRYPT_MIGRATION_PHASE3.md`

---

## Recommended Developer Workflow

### Step 1: Start with Phase 1 docs
If you are new to the migration, begin here.

Why:
- it gives the architecture and validation baseline
- it ensures the repo can build cleanly with a C++ wrapper
- it avoids testing a port before the environment is stable

---

### Step 2: Implement or review Phase 2
Once Phase 1 is stable, move to the algorithm rewrite.

Focus areas:
- memory management and alignment
- endian helpers
- Salsa20 and BlockMix
- PWXForm transform
- SMix loops
- KDF integration

Do not skip test vector validation.

---

### Step 3: Optimize in Phase 3 only after correctness is stable
Do not optimize before correctness is proven.

Rule of thumb:
- Phase 2: correctness first
- Phase 3: performance second

---

## Development Principles

### 1. Preserve behavior first
The C algorithm is the reference. Any C++ rewrite must match behavior exactly.

### 2. Add tests before optimization
Every optimization should be accompanied by a correctness check and benchmark validation.

### 3. Keep API compatibility
Use the same external behavior and, when possible, keep the same symbols and interfaces.

### 4. Use compiler and platform features safely
- use CPU feature detection
- keep fallback paths
- avoid architecture-specific assumptions without detection

### 5. Benchmark deliberately
Performance work without a baseline is guesswork.

---

## Validation Checklist

Before every merge, verify:

### Functional checks
- [ ] legacy C output still matches expected vectors
- [ ] Phase 2 outputs remain deterministic
- [ ] no behavioral regression in release builds
- [ ] memory-hard behavior remains correct

### Hardening checks
- [ ] ASAN passes
- [ ] UBSAN passes
- [ ] TSan passes where applicable
- [ ] no memory leaks or invalid frees

### Performance checks
- [ ] benchmark runs successfully
- [ ] no regression beyond accepted threshold
- [ ] release build uses the optimized path correctly

### Build checks
- [ ] all platforms build successfully
- [ ] compiler warnings are addressed
- [ ] code passes formatting and lint checks

---

## File Reference Map

### Core algorithm files
- `src/crypto/yescrypt/yescrypt-opt_c.h` — non-SIMD reference code
- `src/crypto/yescrypt/yescrypt-best_c.h` — selection of best implementation
- `src/crypto/yescrypt/yescrypt-platform_c.h` — platform memory helpers

### Migration docs
- `YESCRYPT_MIGRATION_PHASE1_CICD.md`
- `YESCRYPT_MIGRATION_PHASE2.md`
- `YESCRYPT_MIGRATION_PHASE3.md`

### PR templates
- `.github/pull_request_template_phase1.md`
- `.github/pull_request_template_phase2.md`

---

## Suggested Developer Order

If you are working in order:

1. Phase 1: build and validate wrapper
2. Phase 2: port and validate algorithm behavior
3. Phase 3: optimize and benchmark
4. merge with validation evidence

Do not skip from Phase 1 directly to Phase 3.

---

## Common Pitfalls

### Pitfall 1: optimizing before correctness is proven
This creates silent regressions and wasted work.

### Pitfall 2: breaking ABI compatibility
Keep public-facing interfaces stable and test them across the migration.

### Pitfall 3: platform assumptions without detection
This breaks builds on unsupported hardware or compilers.

### Pitfall 4: benchmarking without clarity
Use consistent benchmark conditions and document the baseline.

### Pitfall 5: ignoring sanitizer output
This is where subtle memory or UB bugs often appear.

---

## Success Criteria for the Full Migration

The migration is complete when:
- the codebase builds cleanly with the C++ layer
- the pure C++ algorithm matches the legacy implementation
- optimized builds meet or exceed performance targets
- all validation suites pass on supported platforms
- benchmark and regression gates are part of CI
- the maintainers are confident in the final result

---

## Quick Start Commands

### Build the repo
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

### Run unit tests
```bash
ctest --build build --output-on-failure
```

### Run benchmark suite
```bash
./build/src/test/crypto/yescrypt_benchmark --benchmark_out=results.json --benchmark_out_format=json
```

### Run sanitizer build
```bash
cmake -B build-san -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address -g"
cmake --build build-san
ASAN_OPTIONS=detect_leaks=1 ctest --build build-san --output-on-failure
```

---

## Final Recommendation

The migration should be treated as three disciplined phases:
- Phase 1: integration and automation
- Phase 2: correctness and maintainability
- Phase 3: production optimization and hardening

This keeps the work incremental, auditable, and safe.

If you need a single document to keep open while coding, keep this file plus the relevant phase guide beside it.
