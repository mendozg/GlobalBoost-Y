#!/bin/sh
# Copyright (c) 2026 The GlobalBoost-Y developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
#
# Isolated smoke test for an OpenSSL 3.5+ build with the ML-DSA-44 provider.
# This intentionally does not link to or enable any GlobalBoost wallet path.

set -eu

OPENSSL=${OPENSSL:-openssl}
if ! command -v "$OPENSSL" >/dev/null 2>&1; then
    echo "error: OpenSSL executable not found: $OPENSSL" >&2
    exit 1
fi

workdir=$(mktemp -d "${TMPDIR:-/tmp}/globalboost-mldsa.XXXXXX")
trap 'rm -rf "$workdir"' EXIT HUP INT TERM

printf '%s\n' 'GlobalBoost isolated ML-DSA-44 OpenSSL smoke test' > "$workdir/message"
printf '%s\n' 'altered message' > "$workdir/altered-message"

# Generate a throwaway key pair in a private temporary directory.
"$OPENSSL" genpkey -algorithm ML-DSA-44 -out "$workdir/private.pem"
"$OPENSSL" pkey -in "$workdir/private.pem" -pubout -out "$workdir/public.pem"

# Sign a raw message and verify the resulting signature.
"$OPENSSL" pkeyutl -sign -rawin -inkey "$workdir/private.pem" \
    -in "$workdir/message" -out "$workdir/signature"
"$OPENSSL" pkeyutl -verify -rawin -pubin -inkey "$workdir/public.pem" \
    -in "$workdir/message" -sigfile "$workdir/signature" >/dev/null

# A signature for one message must not verify for another.
if "$OPENSSL" pkeyutl -verify -rawin -pubin -inkey "$workdir/public.pem" \
    -in "$workdir/altered-message" -sigfile "$workdir/signature" \
    >/dev/null 2>&1; then
    echo "error: signature unexpectedly verified for an altered message" >&2
    exit 1
fi

printf '%s\n' "OpenSSL ML-DSA-44 keygen/sign/verify smoke test passed."
