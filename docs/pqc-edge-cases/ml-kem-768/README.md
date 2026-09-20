# ML-KEM-768 Edge-Case Investigation

## Objective

Investigate the behavior of ML-KEM-768 under valid,
modified, malformed, and random input conditions.

The goal is to establish a reproducible robustness baseline
for potential post-quantum cryptography integration.

## Scope

Phase 1 covers:

- EC-01: Valid key generation, encapsulation, and decapsulation
- EC-02: Repeated encapsulation using the same public key
- EC-03: Multiple independently generated keypairs
- EC-04: Single-bit ciphertext modification
- EC-05: Multiple ciphertext modifications
- EC-06: All-zero ciphertext
- EC-07: Random ciphertext

## Measurements

Each experiment should record:

1. API return value
2. Whether encapsulated and decapsulated shared secrets match
3. Whether ciphertext or key material was modified
4. Observed behavior
5. Security or integration implications

## Important Interpretation

A successful decapsulation API return does not automatically
prove that the ciphertext was valid.

ML-KEM may use implicit rejection, so secret equality must
also be checked.

## Environment

- Algorithm: ML-KEM-768
- Library: liboqs
- Operating system: Ubuntu 24.04 LTS
- Compiler: GCC
- Build system: CMake and Ninja

## Reproducibility

Experiments should be executed from a clean build and
their observed results should be recorded in the results/
directory.
