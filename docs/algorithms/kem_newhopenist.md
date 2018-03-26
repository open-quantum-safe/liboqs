liboqs nist-branch algorithm datasheet: `kem_newhopenist`
=========================================================

Summary
-------

- **Name**: NewHopeNIST
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: ring learning with errors (RLWE)
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/NewHope.zip
- **Submitters (to NIST competition)**: Thomas Poppelmann, Erdem Alkim, Roberto Avanzi, Joppe Bos, Léo Ducas, Antonio de la Piedra, Peter Schwabe, Douglas Stebila
- **Submitters' website**: http://newhopecrypto.org/
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set       |   OQS algorithm name   | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------|:----------------------:|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| NewHope512-CCA-KEM  |  `newhope_512_cca_kem` |     IND-CCA    |              1              |           928           |           1888          |           1120          |             32             |
| NewHope1024-CCA-KEM | `newhope_1024_cca_kem` |     IND-CCA    |              5              |           1824          |           3680           |           2208          |             32             |

Implementation
--------------

- **Source of implementation:** https://newhopecrypto.org/data/NewHope_2017_12_21.zip
- **License:** Public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs nist-branch**: x86, x64

Additional comments
-------------------

None.
