liboqs nist-branch algorithm datasheet: `kem_LOTUS`
===================================================

Summary
-------

- **Name:** LOTUS
- **Algorithm type:** Lattice-based cryptosystem
- **Main cryptographic assumption:** Learning With Errors (LWE)
- **NIST submission URL:** https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/LOTUS.zip
- **Submitters (to NIST competition):** Le Trieu Phong, Takuya Hayashi, Yoshinori Aono, Shiho Moriai
- **Submitters' website:** https://www2.nict.go.jp/security/lotus/index.html
- **Added to liboqs by:** Ben Davies

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|:-------------:|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| LOTUS192KEM   | TODO           | TODO                        |                 1025024 |                 1126400 |                    1480 |                         24 |
| LOTUS256KEM   | TODO           | TODO                        |                 1470976 |                 1630720 |                    1800 |                         32 |
| LOTUS128KEM   | TODO           | TODO                        |                  658944 |                  714240 |                    1160 |                         16 |

Implementation
--------------

- **Source of implementation:** https://www2.nict.go.jp/security/lotus/impl.html
- **License:** TODO
- **Language:** C
- **Constant-time:** Unknown
- **Architectures supported in liboqs nist-branch:**

Additional comments
-------------------
