liboqs nist-branch algorithm datasheet: `kem_saber`
===================================================

Summary
-------

- **Name**: SABER
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: module learning with rounding (MLWR)
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/SABER.zip
- **Submitters (to NIST competition)**: Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, Frederik Vercauteren
- **Submitters' website**: https://github.com/Angshumank/SABER
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set  | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| LightSaber-KEM |     IND-CCA    |              1              |           672           |           1568          |           736           |             32             |
| Saber-KEM      |     IND-CCA    |              3              |           992           |           2304          |           1088          |             32             |
| FireSaber-KEM  |     IND-CCA    |              5              |           1312          |           3040          |           1472          |             32             |

Implementation
--------------

- **Source of implementation:** https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/SABER.zip
- **License:** Public domain
- **Language:** C
- **Constant-time:** Unknown
- **Architectures supported in liboqs nist-branch**: x86, x64

Additional comments
-------------------

None.
