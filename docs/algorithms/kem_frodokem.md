liboqs master branch algorithm datasheet: `kem_frodokem`
========================================================

Summary
-------

- **Name**: FrodoKEM
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: learning with errors (LWE)
- **Scheme authors**: Michael Naehrig, Erdem Alkim, Joppe Bos, Léo Ducas, Karen Easterbrook, Brian LaMacchia, Patrick Longa, Ilya Mironov, Valeria Nikolaenko, Christopher Peikert, Ananth Raghunathan, Douglas Stebila
- **Authors' website**: https://frodokem.org/
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
- **Implementation version:** https://github.com/Microsoft/PQCrypto-LWEKE/commit/47da00a91270b6f103232314eef0b891b83bfd3b
- **License:** MIT License
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64

Additional comments
-------------------

The original FrodoKEM implementation includes optimizations that are not currently being built in liboqs:

- ARM
- AVX2
- cSHAKE 4-way hashing
