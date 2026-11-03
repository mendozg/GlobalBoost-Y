# ML-DSA-44 scaffold status

The current `crypto/mldsa44` API is a fail-closed scaffold, not an ML-DSA implementation. Key generation, signing, and verification return failure until a vetted FIPS 204 implementation is integrated.

## Requirements before enabling use

- Select and review a maintained, vetted FIPS 204 backend and pin its version/source.
- Define the exact ML-DSA-44 variant and context/domain-separation rules for this project.
- Validate canonical key and signature serialization, including the FIPS 204 ML-DSA-44 sizes (public key: 1,312 bytes; private key: 2,560 bytes; signature: 2,420 bytes).
- Add backend known-answer vectors, malformed-input tests, and negative verification tests.
- Wire the implementation into the crypto library and test build, then verify supported build configurations.
- Keep consensus/wallet activation disabled until transaction sighash semantics, script/witness rules, and compatibility are specified and independently reviewed.

The API scaffold must not be presented as a usable key, address, signer, verifier, or spend path. In particular, placeholder wallet storage or address/script wrappers are not substitutes for the cryptographic and consensus design.
