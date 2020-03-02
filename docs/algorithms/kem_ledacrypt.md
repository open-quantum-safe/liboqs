liboqs algorithm datasheet: `kem_ledacrypt`
===========================================

Summary
-------

- **Name**: LEDAcrypt
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: quasi-cyclic low-density parity check codes
- **Scheme authors**: Marco Baldi, Alessandro Barenghi, Franco Chiaraluce, Gerardo Pelosi, Paolo Santini
- **Authors' website**: https://www.ledacrypt.org
- **Added to liboqs by**: Douglas Stebila
- **NIST Round 2 submission**

Parameter sets
--------------

| Parameter set     | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|-------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| LEDAcryptKEM-LT12 |     IND-CCA    |              1              |           6520          |            50           |           6544          |             32             |
| LEDAcryptKEM-LT32 |     IND-CCA    |              3              |          12032          |            66           |          12064          |             48             |
| LEDAcryptKEM-LT52 |     IND-CCA    |              5              |          19040          |            82           |          19080          |             64             |

Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean/ based on https://github.com/LEDAcrypt/LEDAcrypt
- **Implementation version:** https://github.com/PQClean/PQClean/commit/bf3ef4ad405e00301cadf1b40bb96609b2968158
- **License:** Public domain
- **Language:** C
- **Constant-time:** No
- **Architectures supported in liboqs master branch**: x86, x64
