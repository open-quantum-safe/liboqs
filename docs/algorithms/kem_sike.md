liboqs algorithm datasheet: `kem_sike`
====================================================

Summary
-------

- **Name**: SIKE
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: (supersingular) isogeny walk problem
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/SIKE-Round2.zip
- **Submitters (to NIST competition)**: David Jao, Reza Azarderakhsh, Matthew Campagna, Craig Costello, Luca De Feo, Basil Hess, Amir Jalali, Brian Koziel, Brian LaMacchia, Patrick Longa, Michael Naehrig, Joost Renes, Vladimir Soukharev, David Urbanik
- **Submitters' website**: http://sike.org/
- **Added to liboqs by**: Christian Paquin

Parameter sets
--------------

| Parameter set         | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|-----------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| SIKE-p434             |     IND-CCA    |              1              |            330          |            374          |            346          |             16             |
| SIKE-p434-compressed  |     IND-CCA    |              1              |            196          |            239          |            209          |             16             |
| SIKE-p503             |     IND-CCA    |              2              |            378          |            434          |            402          |             24             |
| SIKE-p503-compressed  |     IND-CCA    |              2              |            224          |            280          |            248          |             24             |
| SIKE-p610             |     IND-CCA    |              3              |            462          |            524          |            486          |             24             |
| SIKE-p610-compressed  |     IND-CCA    |              3              |            273          |            336          |            297          |             24             |
| SIKE-p751             |     IND-CCA    |              3              |            564          |            644          |            596          |             32             |
| SIKE-p751-compressed  |     IND-CCA    |              3              |            334          |            413          |            363          |             32             |

Implementation
--------------

- **Source of implementation:** https://github.com/Microsoft/PQCrypto-SIDH
- **Implementation version:** v3.2 (https://github.com/microsoft/PQCrypto-SIDH/commit/ebd1c80a8ac35e9ca2ef9680291a8a43b95a3bfa)
- **License:** MIT License
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
