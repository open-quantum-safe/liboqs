liboqs master branch algorithm datasheet: `kem_sike`
====================================================

Summary
-------

- **Name**: SIKE
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: (supersingular) isogeny walk problem
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/SIKE.zip
- **Submitters (to NIST competition)**: David Jao, Reza Azarderakhsh, Matthew Campagna, Craig Costello, Luca De Feo, Basil Hess, Amir Jalali, Brian Koziel, Brian LaMacchia, Patrick Longa, Michael Naehrig, Joost Renes, Vladimir Soukharev, David Urbanik
- **Submitters' website**: http://sike.org/
- **Added to liboqs by**: Christian Paquin

Parameter sets
--------------

| Parameter set   | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|-----------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| Sike-p503       |     IND-CCA    |              1              |            378          |            434          |            402          |             16             |
| Sike-p751       |     IND-CCA    |              3              |            564          |            644          |            596          |             24             |

Implementation
--------------

- **Source of implementation:** https://github.com/Microsoft/PQCrypto-SIDH
- **Implmentation version:** v3.0 (https://github.com/Microsoft/PQCrypto-SIDH/tree/77044b76181eb61c744ac8eb7ddc7a8fe72f6919)
- **License:** MIT License
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64

Additional comments
-------------------

The original Sike implementation includes optimizations that are not currently being built in liboqs. See src/kem/sike/upstream/README for details.
