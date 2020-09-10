Rainbow
=======

- **Algorithm type**: signature
- **Main cryptographic assumption**: multivariable polynomials, unbalanced oil and vinegatr
- **Scheme authors**: Jintai Ding, Ming-Shing Chen, Albrecht Petzoldt, Dieter Schmidt, Bo-Yin Yang
- **Version**: NIST Round 2 submission

Implementation
--------------

- **Source of implementation**: https://github.com/fast-crypto-lab/rainbow-submission-round2
- **Implementation version**: https://github.com/PQClean/PQClean/commit/c50bc64a40ce83639cff54419fce60483a30c430
- **License**: CC0 1.0
- **Constant-time**: Yes
- **Optimizations**: Portable C

Parameter sets
--------------

| Parameter set                  | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|--------------------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| Rainbow-Ia-Classic             |     EUF-CMA    |              1              |          148992         |          92960          |           64           |
| Rainbow-Ia-Cyclic              |     EUF-CMA    |              1              |          58144          |          92960          |           64           |
| Rainbow-Ia-Cyclic-Compressed   |     EUF-CMA    |              1              |          58144          |            64           |           64           |
| Rainbow-IIIc-Classic           |     EUF-CMA    |              3              |          710640         |          511448         |           156          |
| Rainbow-IIIc-Cyclic            |     EUF-CMA    |              3              |          206744         |          511448         |           156          |
| Rainbow-IIIc-Cyclic-Compressed |     EUF-CMA    |              3              |          206744         |            64           |           156          |
| Rainbow-Vc-Classic             |     EUF-CMA    |              5              |         1705536         |         1227104         |           204          |
| Rainbow-Vc-Cyclic              |     EUF-CMA    |              5              |          491936         |         1227104         |           204          |
| Rainbow-Vc-Cyclic-Compressed   |     EUF-CMA    |              5              |          491936         |            64           |           204          |
