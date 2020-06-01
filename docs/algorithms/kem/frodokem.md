FrodoKEM
========

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: learning with errors (LWE)
- **Scheme authors**: Michael Naehrig, Erdem Alkim, Joppe Bos, Léo Ducas, Karen Easterbrook, Brian LaMacchia, Patrick Longa, Ilya Mironov, Valeria Nikolaenko, Christopher Peikert, Ananth Raghunathan, Douglas Stebila
- **Authors' website**: https://frodokem.org/
- **Version**: NIST Round 2 submission

Implementation
--------------

- **Source of implementation**: https://github.com/Microsoft/PQCrypto-LWEKE
- **Implementation version**: https://github.com/Microsoft/PQCrypto-LWEKE/commit/d5bbd0417ba111b08a959c0042a1dcc65fb14a89
- **License**: MIT License
- **Constant-time**: Yes
- **Optimizations**: Portable C with optional use of AVX2 and AESNI instructions (selected at compile-time, enabled by default if available)

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| FrodoKEM-640-AES    |     IND-CCA    |              1              |           9616          |          19888          |           9720          |             16             |
| FrodoKEM-640-SHAKE  |     IND-CCA    |              1              |           9616          |          19888          |           9720          |             16             |
| FrodoKEM-976-AES    |     IND-CCA    |              3              |          15632          |          31296          |          15744          |             24             |
| FrodoKEM-976-SHAKE  |     IND-CCA    |              3              |          15632          |          31296          |          15744          |             24             |
| FrodoKEM-1344-AES   |     IND-CCA    |              5              |          21520          |          43088          |          21632          |             32             |
| FrodoKEM-1344-SHAKE |     IND-CCA    |              5              |          21520          |          43088          |          21632          |             32             |
