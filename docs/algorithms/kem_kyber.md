liboqs master branch algorithm datasheet: `kem_kyber`
===========================================================

Summary
-------

- **Name**: CRYSTALS-Kyber
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Module Learning With Errors (RLWE)
- **Scheme authors**: Roberto Avanzi, Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John M. Schanck, Peter Schwabe, Gregor Seiler, Damien Stehle
- **Authors' website**: https://pq-crystals.org/kyber
- **Added to liboqs by**: Peter Schwabe, Douglas Stebila

Parameter sets
--------------

| Parameter set        | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| Kyber-512-CCA-KEM    |     IND-CCA    |              1              |           736           |          1632           |            800          |             32             |
| Kyber-768-CCA-KEM    |     IND-CCA    |              3              |          1088           |          2400           |           1152          |             32             |
| Kyber-1024-CCA-KEM   |     IND-CCA    |              5              |          1440           |          3168           |           1504          |             32             |

Implementation of Kyber-512-CCA-KEM and Kyber-1024-CCA-KEM
----------------------------------------------------------

- **Source of implementation:** https://github.com/pq-crystals/kyber/
- **Implementation version:** ab996e7460e5356b0e23aa034e7c2fe6922e60e6
- **License:** Public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64

Implementation of Kyber-768-CCA-KEM
-----------------------------------

- **Source of implementation:** https://github.com/PQClean/PQClean/tree/master/crypto_kem/kyber768/clean
- **Implementation version:** 0fa56021d1bd68ba59553ba20415c0f052568891
- **License:** Public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64


Additional comments
-------------------

The original Kyber implementation includes optimizations that are not currently being built in liboqs:

- AVX2
