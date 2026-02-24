# Known Answer Test (KAT) Sources

This directory contains the expected SHA256 hash values used to verify deterministic Known Answer Test (KAT) outputs for liboqs algorithms.

## Directory Structure

- `kem/` — Key Encapsulation Mechanisms
- `sig/` — Digital signature schemes
- `sig_stfl/` — Stateful signature schemes

Each subdirectory contains a `kats.json` file storing expected SHA256 hashes for each supported algorithm.

## Verification Process

The test programs `kat_kem`, `kat_sig`, and `kat_sig_stfl` generate deterministic outputs for each algorithm. During testing:

1. The executable is run for a specific algorithm.
2. Output is normalized to Unix-style line endings.
3. A SHA256 hash of the output is computed.
4. The hash is compared against the corresponding value stored in the appropriate `kats.json` file.

This ensures deterministic behavior and cross-platform consistency.

## Source of KAT Values

For algorithms that participated in the NIST Post-Quantum Cryptography (PQC) standardization process, the KAT procedures follow the NIST submission requirements.

Not all algorithms in liboqs were part of the NIST PQC process. In particular, stateful signature schemes were not included in the NIST PQC standardization. Their KAT values are generated based on the deterministic behavior defined by their respective implementations within liboqs. 
The `kat_sig_stfl` test program produces deterministic outputs for these algorithms, and the resulting SHA256 hashes are stored in `sig_stfl/kats.json`.

If changes are made to KAT generation logic or algorithm implementations, the corresponding hash values in the `kats.json` files must be regenerated.
