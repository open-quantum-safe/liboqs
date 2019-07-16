liboqs master branch algorithm datasheet: `kem_ledacrypt`
=========================================================

Summary
-------

- **Name**: LEDAcrypt
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: low-density parity check decoding
- **Scheme authors**: Marco Baldi, Alessandro Barenghi, Franco Chiaraluce, Gerardo Pelosi, Paolo Santini
- **Authors' website**: https://www.ledacrypt.org/
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| LEDAcrypt-KEM-LT-12 |     IND-CCA    |              1              |           6520          |            26           |           6520          |             32             |
| LEDAcrypt-KEM-LT-32 |     IND-CCA    |              3              |          12032          |            34           |          12032          |             48             |
| LEDAcrypt-KEM-LT-52 |     IND-CCA    |              5              |          19040          |            42           |          19040          |             64             |

Implementation
--------------

- **Source of implementation:** https://github.com/LEDAcrypt/LEDAcrypt, via https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/04a0dbfb72ad6ea9ba220553bba233f352027f6f
- **License:** Public domain
- **Language:** C
- **Constant-time:** No
- **Architectures supported in liboqs master branch**: x86, x64

**Note:** The secret key format in this implementation differs from the NIST Round 2 submission.  In particular an extra byte is added for keeping track of the threshold value (in the original implementation, it was a global variable).  See discussion at https://github.com/PQClean/PQClean/pull/186#issuecomment-503513868.
