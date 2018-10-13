liboqs master branch algorithm datasheet: `kem_newhopenist`
===========================================================

Summary
-------

- **Name**: NewHopeNIST
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Ring Learning With Errors (RLWE)
- **Scheme authors**: Thomas Pöppelmann, Erdem Alkim, Roberto Avanzi, Joppe Bos, Léo Ducas, Antonio de la Piedra, Peter Schwabe, Douglas Stebila
- **Authors' website**: https://newhopecrypto.org/
- **Added to liboqs by**: Shravan Mishra

Parameter sets
--------------

| Parameter set        | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| NewHope-512-CCA-KEM  |     IND-CCA    |              1              |           928           |          1888           |           1120          |             32             |
| NewHope-1024-CCA-KEM |     IND-CCA    |              5              |          1824           |          3680           |           2208          |             32             |

Implementation
--------------

- **Source of implementation:** https://newhopecrypto.org/data/NewHope_2017_12_21.zip
- **Implementation version:** https://newhopecrypto.org/data/NewHope_2017_12_21.zip
- **License:**
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64

Additional comments
-------------------

The original NewHope implementation includes optimizations that are not currently being built in liboqs:

- AVX2
