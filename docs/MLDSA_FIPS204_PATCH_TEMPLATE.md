# Full ML-DSA-44 blockchain wallet patch template

> **Status: design and patch template only. Not implemented consensus rules.** This document describes the work needed for a GlobalBoost-Y wallet whose ML-DSA-44 outputs are validated by the chain. It is not a drop-in patch and must not be used to activate a network. Consensus activation must remain disabled until a complete, reviewed verifier and the associated wallet/transaction code are implemented and tested.
>
> Use the standardized **FIPS 204 ML-DSA-44** algorithm. It is not byte-compatible with the historical round-3 Dilithium2 scheme. Do not include fake key/signature sizes, placeholder cryptography, or verifiers that accept every signature.

## Repository architecture and boundary

The existing wallet and script stack is based on secp256k1. Keep ML-DSA types separate from `CKey` and `CPubKey`; do not reinterpret existing serialized keys. Implement and review the feature as a new script/witness spend path, then add wallet management for that path.

The existing activation scaffold is documented in [`MLDSA_FIPS204_PATCH_TEMPLATE.md`](MLDSA_FIPS204_PATCH_TEMPLATE.md). This document expands that into a full project template. It intentionally specifies required decisions instead of inventing consensus-critical formats.

## Phase 0 — freeze the protocol specification

Before implementation, create a reviewed protocol document, for example `doc/mldsa-44-spend.md`. It must define all of the following without ambiguous or implementation-dependent behavior:

- **Algorithm profile:** FIPS 204 ML-DSA-44, exact encoding profile, deterministic/randomized signing behavior, and permitted context string/domain separation.
- **Output commitment:** exact script/witness version and program length; whether the output commits to the public key or its hash; exact hash function and tagged/domain-separated construction.
- **Witness stack:** exact item count, order, maximum lengths, annex/unknown item treatment, and whether script-path spends exist in the first version.
- **Transaction digest:** exact sighash serialization and which transaction/prevout fields are committed to; define every supported hash-type value and reject unknown values.
- **Signature verification:** exact message bytes passed to ML-DSA, context string, malformed key/signature behavior, and signature failure behavior.
- **Consensus resource limits:** signature/public-key size bounds, hashing and verifier work accounting, transaction weight implications, and DoS protections.
- **Softfork compatibility:** pre-activation behavior and post-activation behavior for the selected output version, including malformed and unknown witness programs.
- **Wallet encoding:** address HRP/version, checksums, network separation, descriptor representation, key import/export and backup format.
- **Migration/activation:** version-bit allocation, deployment name, start/timeout, threshold, minimum activation height, and client rollout policy.

Do not proceed to an activation proposal until the specification is independently reviewed for softfork compatibility and downgrade/ambiguity cases. A new witness version is a design option, not an automatic guarantee of softfork compatibility.

## Phase 1 — isolated crypto API

Proposed paths:

- `src/crypto/mldsa44.h`
- `src/crypto/mldsa44.cpp`
- `src/test/crypto_mldsa44_tests.cpp`

The interface should use byte spans/vectors with lengths validated by the implementation. Keep API declarations narrow and avoid exposing internal polynomial structures to wallet or script callers.

Template API shape (not an implementation):

```cpp
namespace crypto::mldsa44 {

// API contract only: sizes and error/result types must come from the selected
// FIPS 204 implementation and protocol profile; do not guess them here.
struct KeyPair;
struct PublicKey;
struct Signature;

// Implement with a cryptographically secure entropy source for key generation.
// Seed import/derivation is a separate, explicitly specified API.
util::Result<KeyPair> GenerateKeyPair(/* secure entropy source */);

util::Result<Signature> Sign(
    const KeyPair& key,
    Span<const unsigned char> message,
    Span<const unsigned char> context);

bool Verify(
    const PublicKey& key,
    Span<const unsigned char> message,
    Span<const unsigned char> context,
    Span<const unsigned char> signature);

} // namespace crypto::mldsa44
```

Implementation requirements:

- Choose a vetted, portable C++ implementation and document its provenance, license, version, and any local modifications. “Pure C++” describes language/runtime dependencies; it does not replace cryptographic review.
- If implementing the algorithm in-tree, require independent cryptographic review before funds or network consensus use.
- Add FIPS 204 known-answer tests, positive and negative vectors, malformed encoding tests, and cross-checks against an independent implementation.
- Clear private key and expanded-secret buffers using the repository’s secure memory facilities; avoid secret-bearing `std::string` and ordinary heap buffers where practical.
- Use the repository CSPRNG for key generation. Specify and test seed import separately; do not silently invent a BIP32 derivation scheme for ML-DSA.
- Bound all input sizes before allocation or expensive hashing.
- Add fuzz targets for public-key, signature, and serialized-key parsing.

Build integration belongs in `src/Makefile.am` and tests in `src/Makefile.test.include`; update the correct library target after confirming whether the crypto module is used by common, consensus, and wallet targets. Do not place consensus verification in a wallet-only library.

## Phase 2 — script/witness and sighash implementation

Before adding new address or wallet code, implement a standalone consensus path for the specified output/witness format.

Expected areas to inspect and modify after the spec is approved:

- `src/script/script.h` / `src/script/script.cpp`: output template and witness-version parsing, only if required by the chosen design.
- `src/script/interpreter.h` / `src/script/interpreter.cpp`: strict signature verification entry point and script error reporting.
- `src/script/sign.cpp` and relevant sighash implementation: construct exactly the specified digest.
- `src/script/standard.h` / `src/script/standard.cpp`: standardness classification; policy may be stricter than consensus but must not contradict it.
- `src/validation.cpp` and script-flag/deployment plumbing: pass activation state to script checks at the correct contextual layer.
- `src/consensus/` and `src/deploymentstatus.*`: only as required by the selected rule design.

Consensus verifier requirements:

- Parse and validate every witness element canonically, with checked lengths and no integer overflow.
- Reject malformed public keys/signatures and unsupported hash types deterministically.
- Use only the fully specified digest/context construction.
- Enforce resource bounds before calling the verifier; account for CPU cost in policy and block limits.
- Do not silently fall back to ECDSA if ML-DSA parsing or verification fails.
- Keep existing ECDSA/Taproot paths unchanged and cover them with regression tests.
- Make deployment context explicit in validation; do not add a global or context-free activation check inside a script evaluator that lacks the candidate block’s proper deployment state.

No pseudocode verifier should return success. Do not wire incomplete code into block acceptance.

## Phase 3 — deployment and activation

Use the existing VersionBits mechanism only after the rule implementation is complete on a controlled development branch.

Proposed files:

- `src/consensus/params.h`: append a new `DeploymentPos` enumerator, preserving existing indices.
- `src/deploymentinfo.cpp`: add matching metadata in enum order; set `gbt_force` according to the final, reviewed template policy.
- `src/kernel/chainparams.cpp`: explicit parameters for main/testnet/signet/regtest. Regtest may use `ALWAYS_ACTIVE` for integration testing only after the verifier is operational; public networks stay `NEVER_ACTIVE` until their coordinated launch plan is approved.
- `src/rpc/blockchain.cpp`: expose the deployment in `getdeploymentinfo`.
- `src/test/versionbits_tests.cpp`: assert unique version-bit allocation and expected deployment states for every network.

Do not advertise activation or set `gbt_force` for rules that are not implemented. Do not choose a production version bit without checking all current and planned deployments and coordinating the allocation.

## Phase 4 — wallet key model and persistence

Proposed files (names are examples; follow the repository’s style after inspecting key-manager abstractions):

- `src/wallet/mldsa44_key.h/.cpp`: dedicated key types and safe parse/serialize helpers.
- `src/wallet/`: scriptPubKey manager/keypool integration, wallet database records, backup/restore, key encryption and unlock flows.
- `src/interfaces/wallet.h` and wallet interface implementation: add API only where needed.
- `src/wallet/rpc/`: wallet RPC commands or extensions for address generation, key import/export if supported, and transaction signing.
- `src/qt/`: GUI changes only if this wallet is exposed in the Qt client.

Requirements:

- Store an explicit algorithm/version tag with every key record; preserve old wallet compatibility.
- Store secrets with the existing encrypted-wallet protections and secure allocators/cleanse mechanisms.
- Define backup, restore, locked-wallet, keypool, rescanning, and crash-safe database behavior.
- Do not promise HD derivation or seed compatibility until a derivation scheme is specified and tested.
- Ensure wallet discovery can identify outputs without requiring private key exposure.

## Phase 5 — addresses, descriptors, and transaction signing

After the consensus output format is fixed:

- Define a distinct network-aware address encoding and add encode/decode and invalid-input tests.
- Add a descriptor or similarly explicit output representation; update descriptor parsing, inference, checksums, and range handling as needed.
- Add wallet coin ownership, destination creation, transaction construction, fee/weight estimation, signing, PSBT support if applicable, and signature finalization.
- Make signer and verifier serialize the digest identically; test transactions generated by one implementation and checked by another.
- Add RPC/GUI behavior with clear algorithm identification. Do not make an ML-DSA address look like an existing secp256k1 address.

## Phase 6 — test and release gates

Required test layers:

1. Crypto unit tests: FIPS 204 KATs, cross-implementation vectors, invalid inputs, secret-memory/error paths.
2. Script tests: valid spends, bad signatures, wrong keys, wrong digest/context, malformed/oversized witness, unsupported hash types, pre-activation semantics.
3. Consensus tests: block acceptance and rejection on both sides of activation; deployment transitions; reorgs across activation; historical behavior unchanged.
4. Mempool/policy tests: standardness, weight/fee calculation, resource limits, replacement/package behavior where relevant.
5. Wallet tests: create/import, encrypt/unlock, database roundtrip, backup/restore, address derivation, signing, restart, rescans, mixed key wallet behavior.
6. Functional regtest tests: complete create-output/spend-confirm flow, and negative cases rejected by both mempool and block validation.
7. Fuzzing and sanitizers: crypto parsers, witness parser, transaction/script paths, and wallet deserialization.
8. Independent review: cryptography, consensus specification, implementation, and activation plan.

Suggested milestones:

- M0: reviewed protocol specification; no consensus code enabled.
- M1: crypto module + vectors, no wallet or consensus dependency.
- M2: script/witness verifier behind inactive deployment; public networks cannot activate.
- M3: wallet key storage, address, descriptors, and signing; end-to-end regtest tests.
- M4: regtest always-active integration after the complete implementation passes tests.
- M5: test network rollout with coordinated activation parameters and independent review.
- M6: mainnet activation only after deployment coordination, upgrade guidance, and release sign-off.

## Explicit non-goals for the first template patch

- No fake or placeholder key generation/signing/verification.
- No fabricated ML-DSA key or signature lengths.
- No modifications that reinterpret secp256k1 `CKey`/`CPubKey` data.
- No regtest activation of a verifier that does not exist.
- No mainnet/testnet/signet activation parameters or claimed activation date.
- No claim that a deployment enum alone makes outputs spendable.

## Review checklist

- [ ] Protocol document specifies a unique, canonical serialization and digest.
- [ ] Softfork compatibility is demonstrated against the pre-activation rule set.
- [ ] Algorithm implementation/version/license/provenance are documented.
- [ ] FIPS 204 and cross-implementation test vectors pass.
- [ ] Existing secp256k1 wallet and consensus tests remain unchanged and pass.
- [ ] Parser and verifier have deterministic resource bounds.
- [ ] Consensus, mempool, wallet, and RPC paths agree on exact witness rules.
- [ ] Deployment bit allocation and per-network parameters are reviewed.
- [ ] Regtest activation occurs only after end-to-end validation is implemented.
- [ ] Independent cryptographic and consensus reviews are complete.
