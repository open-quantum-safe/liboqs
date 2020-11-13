SABER
=====

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: module learning with rounding
- **Scheme authors**: Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, Frederic Vercauteren
- **Authors' website**: https://www.esat.kuleuven.be/cosic/pqcrypto/saber/
- **Version**: NIST Round 3 submission

Implementation
--------------

- **Source of implementation**: https://github.com/KULeuven-COSIC/SABER/tree/509cc5ec3a7e12a751ccdd2ef5bd6e54e00bd350 via https://github.com/jschanck/package-pqclean/tree/1ae84c3c/saber
- **Implementation version**: https://github.com/PQClean/PQClean/commit/3c8be3cb1f8ca0953f2df44ea665f118efb468d6
- **License**: Public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C, with AVX2 instructions (if available at runtime)

Parameter sets
--------------

| Parameter set  | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| LightSaber-KEM |     IND-CCA    |              1              |           672           |           1568          |           736           |             32             |
| Saber-KEM      |     IND-CCA    |              3              |           992           |           2304          |           1088          |             32             |
| FireSaber-KEM  |     IND-CCA    |              5              |           1312          |           3040          |           1472          |             32             |
