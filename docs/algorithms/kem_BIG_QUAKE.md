liboqs nist-branch algorithm datasheet: `kem_BIG_QUAKE`
======================================================

Summary
-------

- **Name**: BIG QUAKE
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: module learning with errors (MLWE)
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/BUG_QUAKE.zip
- **Submitters (to NIST competition)**: Alain Couvreur Magali Bardet Elise Barelli Olivier Blazy Rodolfo Canto-Torres Philippe Gaborit Ayoub Otmani Nicolas Sendrier Jean-Pierre Tillich 
- **Submitters' website**: https://bigquake.inria.fr/
- **Added to liboqs by**: Shravan Mishra 

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| BIG_QUAKE_1   |     IND-CPA    |              1              |           25482         |           14772         |           201           |             32             |
| BIG_QUAKE_3   |     IND-CPA    |              3              |           84132         |           30860         |           3248          |             32             |
| BIG_QUAKE_5   |     IND-CPA    |              5              |           149800        |           41804         |           3932          |             32             |

Implementation
--------------

- **Source of implementation:**  https://bigquake.inria.fr/
- **License:** Public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs nist-branch**: x86, x64

