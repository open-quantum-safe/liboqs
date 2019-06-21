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
- **NIST Round 2 submission**

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| FrodoKEM-640-AES    |     IND-CCA    |              1              |           9616          |          19888          |           9720          |             16             |
| FrodoKEM-640-SHAKE  |     IND-CCA    |              1              |           9616          |          19888          |           9720          |             16             |
| FrodoKEM-976-AES    |     IND-CCA    |              3              |          15632          |          31296          |          15744          |             24             |
| FrodoKEM-976-SHAKE  |     IND-CCA    |              3              |          15632          |          31296          |          15744          |             24             |
| FrodoKEM-1344-AES   |     IND-CCA    |              5              |          21520          |          43088          |          21632          |             32             |
| FrodoKEM-1344-SHAKE |     IND-CCA    |              5              |          15632          |          43088          |          21632          |             32             |

Implementation
--------------

- **Source of implementation:** https://github.com/Microsoft/PQCrypto-LWEKE/ via https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/ba04db65d7d21e52dea59651bfff4ad28653852d
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
