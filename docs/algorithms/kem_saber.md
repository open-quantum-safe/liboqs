liboqs master branch algorithm datasheet: `kem_saber`
=====================================================

Summary
-------

- **Name**: SABER
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: module learning with rounding
- **Scheme authors**: Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, Frederic Vercauteren
- **Authors' website**: https://www.esat.kuleuven.be/cosic/pqcrypto/saber/
- **Added to liboqs by**: Douglas Stebila
- **NIST Round 2 submission**

Parameter sets
--------------

| Parameter set  | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| LightSaber-KEM |     IND-CCA    |              1              |           672           |           1568          |           736           |             32             |
| Saber-KEM      |     IND-CCA    |              3              |           992           |           2304          |           1088          |             32             |
| FireSaber-KEM  |     IND-CCA    |              5              |           1312          |           3040          |           1472          |             32             |

Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean/ based on https://github.com/KULeuven-COSIC/SABER/commit/14ede83f1ff3bcc41f0464543542366c68b55871
- **Implementation version:** https://github.com/PQClean/PQClean/commit/0ed5ba4a30ee509f8227ced5739d8359b5380eb1
- **License:** Public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
