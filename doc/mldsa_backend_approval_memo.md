# OpenSSL ML-DSA backend approval memo

Status: pending approval

## Summary

This repository remains in a fail-closed state. The ML-DSA backend is not enabled and no wallet, signing, verification, or transaction flow currently uses it.

The proposed future backend is the OpenSSL 3.5+ ML-DSA provider.

## Rationale

OpenSSL is the preferred candidate because it provides:

- a mature, reviewed crypto provider model
- explicit provider selection and isolation
- standardized ML-DSA support
- a clearer security and compatibility review path than custom or opaque third-party code

## Required review items

- [ ] exact OpenSSL version identified
- [ ] license compatibility confirmed
- [ ] exact ML-DSA variant documented
- [ ] supported key sizes and parameter sets confirmed
- [ ] cryptographic review complete
- [ ] test vectors validated
- [ ] FIPS/compliance status reviewed
- [ ] build compatibility confirmed
- [ ] side-channel and memory handling reviewed
- [ ] wallet/signing integration design reviewed
- [ ] security signoff obtained

## Hard block conditions

The backend must remain disabled if any of the following are true:

- provider version is not explicitly known
- license review is incomplete
- ML-DSA variant is ambiguous
- key sizes or signing semantics are undocumented
- security review is not complete
- wallet or spend integration is attempted before approval

## Final policy

Fail closed. No ML-DSA backend activation without explicit security and design approval.
