# Isolated OpenSSL ML-DSA-44 smoke test

This smoke test lets maintainers exercise an installed OpenSSL ML-DSA provider
without linking it into GlobalBoost or enabling any wallet, consensus, or
transaction signing path. It generates a throwaway key in a temporary directory,
signs a test message, checks that verification succeeds, and checks that the
signature does not verify for a different message. Temporary key material is
removed when the script exits.

Run it with OpenSSL 3.5 or later that provides ML-DSA-44:

```sh
sh test/mldsa_openssl_smoke.sh
```

To use a specific OpenSSL executable:

```sh
OPENSSL=/path/to/openssl sh test/mldsa_openssl_smoke.sh
```

This is an environment/provider smoke test only. It does not test a
GlobalBoost backend adapter, establish FIPS validation, prove side-channel
properties, or authorize production use. The project ML-DSA API and backend
remain fail-closed until a separately reviewed adapter and test suite are
implemented.
