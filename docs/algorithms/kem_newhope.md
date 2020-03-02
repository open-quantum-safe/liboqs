liboqs algorithm datasheet: `kem_newhope`
=======================================================

Summary
-------

- **Name**: NewHope
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Ring Learning With Errors (RLWE)
- **Scheme authors**: Thomas Pöppelmann, Erdem Alkim, Roberto Avanzi, Joppe Bos, Léo Ducas, Antonio de la Piedra, Peter Schwabe, Douglas Stebila
- **Authors' website**: https://newhopecrypto.org/
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set        | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| NewHope-512-CCA      |     IND-CCA    |              1              |           928           |          1888           |           1120          |             32             |
| NewHope-1024-CCA     |     IND-CCA    |              5              |          1824           |          3680           |           2208          |             32             |

Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/bf3ef4ad405e00301cadf1b40bb96609b2968158
- **License:** Public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64

Additional comments
-------------------

The original NewHope implementation includes optimizations that are not currently being built in liboqs:

- AVX2
