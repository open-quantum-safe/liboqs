liboqs master branch algorithm datasheet: `kem_ledacrypt`
=========================================================

Summary
-------

- **Name**: LEDAcrypt
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Decoding in quasi-cyclic low-density parity-check codes
- **Scheme authors**: Marco Baldi, Alessandro Barenghi, Franco Chiaraluce, Gerardo Pelosi, Paolo Santini
- **Authors' website**: https://www.ledacrypt.org
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set           | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|-------------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| LEDALEDAcrypt-KEM-LT-12 |     IND-CCA    |              1              |           6520          |            26           |           6520          |             32             |

Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean, based on NIST Round 2 submission
- **Implementation version:** https://github.com/PQClean/PQClean/commit/b8a243bf2d541f52a5870a841a2f9bc5b5e780a2
- **License:** TODO
- **Language:** C
- **Constant-time:** No
- **Architectures supported in liboqs master branch**: x86, x64
