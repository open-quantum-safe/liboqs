SABER
=====

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: module learning with rounding
- **Scheme authors**: Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, Frederic Vercauteren
- **Authors' website**: https://www.esat.kuleuven.be/cosic/pqcrypto/saber/
- **Version**: NIST Round 2 submission

Implementation
--------------

- **Source of implementation**: https://github.com/KULeuven-COSIC/SABER
- **Implementation version**: https://github.com/PQClean/PQClean/commit/c50bc64a40ce83639cff54419fce60483a30c430
- **License**: Public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C

Parameter sets
--------------

| Parameter set  | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| LightSaber-KEM |     IND-CCA    |              1              |           672           |           1568          |           736           |             32             |
| Saber-KEM      |     IND-CCA    |              3              |           992           |           2304          |           1088          |             32             |
| FireSaber-KEM  |     IND-CCA    |              5              |           1312          |           3040          |           1472          |             32             |
