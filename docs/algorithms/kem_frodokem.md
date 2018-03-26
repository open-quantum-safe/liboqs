liboqs nist-branch algorithm datasheet: `kem_frodokem`
======================================================

Summary
-------

- **Name**: FrodoKEM
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: learning with errors (LWE)
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/Frodo.zip
- **Submitters (to NIST competition)**: Michael Naehrig, Erdem Alkim, Joppe Bos, Léo Ducas, Karen Easterbrook, Brian LaMacchia, Patrick Longa, Ilya Mironov, Valeria Nikolaenko, Christopher Peikert, Ananth Raghunathan, Douglas Stebila
- **Submitters' website**: https://frodokem.org/
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| FrodoKEM-640-AES    |     IND-CCA    |              1              |           9616          |          19872          |           9736          |             16             |
| FrodoKEM-640-cSHAKE |     IND-CCA    |              1              |           9616          |          19872          |           9736          |             16             |
| FrodoKEM-976-AES    |     IND-CCA    |              3              |          15632          |          31272          |          15768          |             24             |
| FrodoKEM-976-cSHAKE |     IND-CCA    |              3              |          15632          |          31272          |          15768          |             24             |

Implementation
--------------

- **Source of implementation:** https://github.com/Microsoft/PQCrypto-LWEKE
- **License:** MIT License
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs nist-branch**: x86, x64

Additional comments
-------------------

The original FrodoKEM implementation includes optimizations that are not currently being built in liboqs:

- AES-NI
- AVX2
