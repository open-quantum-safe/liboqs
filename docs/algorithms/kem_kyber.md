liboqs algorithm datasheet: `kem_kyber`
=====================================================

Summary
-------

- **Name**: CRYSTALS-Kyber
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Module Learning With Errors (RLWE)
- **Scheme authors**: Roberto Avanzi, Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John M. Schanck, Peter Schwabe, Gregor Seiler, Damien Stehle
- **Authors' website**: https://pq-crystals.org/kyber
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| Kyber512      |     IND-CCA    |              1              |           800           |          1632           |            736          |             32             |
| Kyber768      |     IND-CCA    |              3              |          1184           |          2400           |           1088          |             32             |
| Kyber1024     |     IND-CCA    |              5              |          1568           |          3168           |           1568          |             32             |
| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| Kyber512-90s  |     IND-CCA    |              1              |           800           |          1632           |            736          |             32             |
| Kyber768-90s  |     IND-CCA    |              3              |          1184           |          2400           |           1088          |             32             |
| Kyber1024-90s |     IND-CCA    |              5              |          1568           |          3168           |           1568          |             32             |

Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/298dd2cf223cff50cc53bea534474796f9960653
- **License:** Public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
