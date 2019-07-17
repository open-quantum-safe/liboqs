liboqs algorithm datasheet: `kem_ntskem`
========================================

Summary
-------

- **Name**: NTS-KEM
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: one-wayness of McEliece, decoding random linear codes
- **Scheme authors**: Martin Albrecht, Carlos Cid, Kenneth G. Paterson, Cen Jung Tjhai, Martin Tomlinson
- **Authors' website**: https://nts-kem.io
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set  | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| NTS-KEM(12,64) |     IND-CCA    |              1              |          319488         |           9248          |           128           |             32             |

Implementation
--------------

- **Source of implementation:** https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/NTS-KEM-Round2.zip reference implementation via https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/27f932ff4c87a53b616cf4fff820ad581114be94
- **License:** MIT License
- **Language:** C
- **Constant-time:** Unknown
- **Architectures supported in liboqs master branch**: x86, x64

**Note** This implementation does not yet meet the requirements of liboqs, and is not being merged at this time; it is solely here for testing purposes.
