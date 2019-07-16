liboqs algorithm datasheet: `sig_rainbow`
=========================================

Summary
-------

- **Name**: Rainbow
- **Algorithm type**: signature
- **Main cryptographic assumption**: multi-variate quadratic
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/Rainbow-Round2.zip
- **Submitters (to NIST competition)**: Jintai Ding, Ming-Shing Chen, Albrecht Petzoldt, Dieter Schmidt, Bo-Yin Yang
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

| Parameter set                  | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|--------------------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| Rainbow-Ia-Classic             |     EUF-CMA    |              1              |          148992         |          92960          |           64           |
| Rainbow-Ia-Cyclic              |     EUF-CMA    |              1              |           58144         |          92960          |           64           |
| Rainbow-Ia-Cyclic-Compressed   |     EUF-CMA    |              1              |           58144         |            64           |           64           |
| Rainbow-IIIc-Classic           |     EUF-CMA    |              3              |          710640         |          511448         |           156          |
| Rainbow-IIIc-Cyclic            |     EUF-CMA    |              3              |          206744         |          511448         |           156          |
| Rainbow-IIIc-Cyclic-Compressed |     EUF-CMA    |              3              |          206744         |            64           |           156          |
| Rainbow-Vc-Classic             |     EUF-CMA    |              5              |         1705536         |         1227104         |           204          |
| Rainbow-Vc-Cyclic              |     EUF-CMA    |              5              |          491936         |         1227104         |           204          |
| Rainbow-Vc-Cyclic-Compressed   |     EUF-CMA    |              5              |          491936         |            64           |           204          |

Implementation
--------------

- **Source of implementation:** https://github.com/PQClean/PQClean, originally from https://github.com/fast-crypto-lab/rainbow-submission-round2/commit/af826fcb78f6af51a02d0352cff28a9690467bfd
- **Implementation version:** https://github.com/PQClean/PQClean/pull/182/commits/fcbf6d98f230fc624e2b461942d73605cf261025
- **License:** CC0 1.0 Universal
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs master branch**: x86, x64
