# FIPS 204 (ML-DSA-44) consensus integration patch template

> **Status: design/template only.** This document is not an implemented or activated consensus change. The snippets below are an activation scaffold; they do not add ML-DSA signing, verification, address handling, or spendable outputs. Do not treat the feature as usable for funds until the cryptographic implementation and all consensus integration have been independently reviewed and tested.

This project uses C++ and VersionBits/BIP9 deployments. The intent is to use **FIPS 204 ML-DSA-44**, not the incompatible round-3 Dilithium2 variant. Regtest should report the deployment as active from its first block. Mainnet, testnet, and signet remain inactive until a reviewed activation schedule is chosen.

## 1. Add a VersionBits deployment slot

`src/consensus/params.h` — append the new deployment so existing deployment indices remain unchanged:

```diff
 enum DeploymentPos : uint16_t {
     DEPLOYMENT_TESTDUMMY,
     DEPLOYMENT_TAPROOT, // Deployment of Schnorr/Taproot (BIPs 340-342)
+    DEPLOYMENT_MLDSA, // FIPS 204 ML-DSA-44
     // NOTE: Also add new deployments to VersionBitsDeploymentInfo in deploymentinfo.cpp
     MAX_VERSION_BITS_DEPLOYMENTS
 };
```

## 2. Register the name

`src/deploymentinfo.cpp` — append an entry matching the enum order:

```diff
     {
         /*.name =*/ "taproot",
         /*.gbt_force =*/ true,
     },
+    {
+        /*.name =*/ "mldsa",
+        /*.gbt_force =*/ false,
+    },
 };
```

`gbt_force` is deliberately `false` in this template: miners and template clients must not be told to force a consensus rule that is not implemented and reviewed yet.

## 3. Configure each network

Use a distinct version bit (the template uses bit 3; verify it is not assigned elsewhere before merging). Set the deployment explicitly on all networks. Keep public networks inactive until a real activation schedule is approved.

`src/kernel/chainparams.cpp`:

```diff
         consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 443000; //
+
+        consensus.vDeployments[Consensus::DEPLOYMENT_MLDSA].bit = 3;
+        consensus.vDeployments[Consensus::DEPLOYMENT_MLDSA].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
+        consensus.vDeployments[Consensus::DEPLOYMENT_MLDSA].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
+        consensus.vDeployments[Consensus::DEPLOYMENT_MLDSA].min_activation_height = 0;
```

Apply the same four assignments in the `CTestNetParams` and `SigNetParams` constructors. For `CRegTestParams`, use `ALWAYS_ACTIVE` instead of `NEVER_ACTIVE`:

```diff
         consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay
+
+        consensus.vDeployments[Consensus::DEPLOYMENT_MLDSA].bit = 3;
+        consensus.vDeployments[Consensus::DEPLOYMENT_MLDSA].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
+        consensus.vDeployments[Consensus::DEPLOYMENT_MLDSA].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
+        consensus.vDeployments[Consensus::DEPLOYMENT_MLDSA].min_activation_height = 0; // No activation delay
```

Regtest's existing `-vbparams` option can override the deployment timing for activation-state tests. This configuration alone does **not** make an ML-DSA output spendable or add any signature validation.

## 4. Expose deployment status in RPC

`src/rpc/blockchain.cpp`, inside `DeploymentInfo()`:

```diff
     SoftForkDescPushBack(blockindex, softforks, chainman, Consensus::DEPLOYMENT_TAPROOT);
+    SoftForkDescPushBack(blockindex, softforks, chainman, Consensus::DEPLOYMENT_MLDSA);
     return softforks;
```

## 5. Add a configuration regression test

Add to `src/test/versionbits_tests.cpp` (and include `<deploymentinfo.h>`):

```cpp
BOOST_AUTO_TEST_CASE(mldsa_deployment_parameters)
{
    const auto regtest = CreateChainParams(*m_node.args, CBaseChainParams::REGTEST);
    const auto& deployment = regtest->GetConsensus().vDeployments[Consensus::DEPLOYMENT_MLDSA];

    BOOST_CHECK_EQUAL(deployment.nStartTime, Consensus::BIP9Deployment::ALWAYS_ACTIVE);
    BOOST_CHECK_EQUAL(deployment.nTimeout, Consensus::BIP9Deployment::NO_TIMEOUT);
    BOOST_CHECK_EQUAL(DeploymentName(Consensus::DEPLOYMENT_MLDSA), "mldsa");

    for (const auto& network : {CBaseChainParams::MAIN, CBaseChainParams::TESTNET, CBaseChainParams::SIGNET}) {
        const auto params = CreateChainParams(*m_node.args, network);
        const auto& public_deployment = params->GetConsensus().vDeployments[Consensus::DEPLOYMENT_MLDSA];
        BOOST_CHECK_EQUAL(public_deployment.nStartTime, Consensus::BIP9Deployment::NEVER_ACTIVE);
        BOOST_CHECK_EQUAL(public_deployment.nTimeout, Consensus::BIP9Deployment::NO_TIMEOUT);
    }
}
```

The `m_node.args` fixture access follows the existing versionbits tests. Confirm the include for `CBaseChainParams` is available in this file or include `<chainparamsbase.h>` as needed by the branch.

## 6. Required before enabling ML-DSA spends

The activation scaffold must not be mistaken for a complete softfork. Before any network is configured to activate spend rules, implementation work must specify and test at least:

- A canonical transaction/script encoding for ML-DSA-44 public keys, signatures, and sighashes; do not reuse `CKey` or `CPubKey`, which encode secp256k1 keys.
- Consensus verification that rejects malformed keys, signatures, and encodings deterministically, with bounded resource use and no permissive fallback.
- How the new rule is selected from the active deployment state in the block/script-validation context. Do not put a context-free deployment check into `EvalScript`.
- Mempool policy, transaction weight/sigop accounting, wallet key storage and backup, address encoding, RPC/GUI behavior, and network interoperability.
- FIPS 204 known-answer tests, negative tests, cross-implementation vectors, fuzzing, and an independent cryptographic/consensus review.
- Activation/version-bit allocation coordination and a network-specific rollout plan before mainnet/testnet/signet activation.

**Do not add a placeholder verifier that returns success, a fake signature routine, or fabricated key sizes.** Keep ML-DSA transaction acceptance disabled until the real verifier and its consensus integration are complete.
