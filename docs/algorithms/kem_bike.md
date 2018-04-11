liboqs nist-branch algorithm datasheet: `kem_bike`
======================================================

Summary
-------

- **Name**: BIKE
- **Algorithm type**: Key Encapsulation Mechanism
- **Main cryptographic assumption**: Quasi Cyclic Syndrom Decoding (QCSD)
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/BIKE.zip
- **Submitters (to NIST competition)**: Nicolas Aragon, Paulo Barreto, Slim Bettaieb, Loic Bidoux, Olivier Blazy, Jean-Christophe Deneuville, Phillipe Gaborit, Shay Gueron, Tim Guneysu, Carlos Aguilar Melchor, Rafael Misoczki, Edoardo Persichetti, Nicolas Sendrier, Jean-Pierre Tillich, Gilles Zemor
- **Submitters' website**: http://bikesuite.org/
- **Added to liboqs by**: Shay Gueron and Nir Drucker.

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| BIKE1-L1            |     IND-CPA    |              1              |           2542          |          2542          |           2542          |             32             |
| BIKE1-L3            |     IND-CPA    |              3              |           4964          |          4964          |           4964          |             32             |
| BIKE1-L5            |     IND-CPA    |              5              |           8188          |          8188          |           8188          |             32             |
| BIKE2-L1            |     IND-CPA    |              1              |           2542          |          2542          |           2542          |             32             |
| BIKE2-L3            |     IND-CPA    |              3              |           4964          |          4964          |           4964          |             32             |
| BIKE2-L5            |     IND-CPA    |              5              |           8188          |          8188          |           8188          |             32             |
| BIKE3-L1            |     IND-CPA    |              1              |           2758          |          2758          |           2758          |             32             |
| BIKE3-L3            |     IND-CPA    |              3              |           5422          |          5422          |           5422          |             32             |
| BIKE3-L5            |     IND-CPA    |              5              |           9034          |          9034          |           9034          |             32             |

Implementation
--------------

- **Source of implementation:** This is a reference code (for functionality testing) that is based on the reference code of the Nist submission. This code was modified to use OpenSSL instead of NTL.
- **License:** MIT License
- **Language:** C
 **Constant-time:** No
 **Architectures supported in liboqs nist-branch**: x86, x64

Additional comments
-------------------
- The original BIKE implementation includes additional optimizations that are not currently being built in liboqs:
  - CLMUL
  - AES-NI
  - AVX2
  - AVX512

- BIKE2 is currently disabled.


