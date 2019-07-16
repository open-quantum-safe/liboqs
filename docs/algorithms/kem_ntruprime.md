liboqs master branch algorithm datasheet: `kem_ntruprime`
=========================================================

Summary
-------

- **Name**: NTRU Prime
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: rounded NTRU prime
- **Scheme authors**: Daniel J. Bernstein, Chitchanok Chuengsatiansup, Tanja Lange, Christine van Vredendaal
- **Authors' website**: https://ntruprime.cr.yp.to
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| ntrulpr653    |     IND-CCA    |              2              |           897           |           1125          |           1025          |             32             |
| ntrulpr761    |     IND-CCA    |              3              |           1039          |           1294          |           1167          |             32             |
| ntrulpr857    |     IND-CCA    |              4              |           1184          |           1463          |           1312          |             32             |

Implementation
--------------

- **Source of implementation:** https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/NTRU-Prime-Round2.zip reference implementation, via https://github.com/PQClean/PQClean
- **Implementation version:** https://github.com/PQClean/PQClean/commit/34d52e93d65c9b958552059e55e4f532dbc72a55
- **License:** Public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64

**Note**: This version contains modifications compared to the original reference implementation, including changes in Encode and Decode due to incompatibilities with the Microsoft Visual Studio C compiler.  Dan Bernstein has indicated that a future optimized version of the upstream release will have unrolled functions that should have the effect of eliminating the need for those changes.  The NTRU Prime code in PQClean has not been merged pending this release, and similarly remains on a branch in liboqs pending that release.
